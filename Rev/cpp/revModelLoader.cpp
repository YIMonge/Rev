#include "revModelLoader.h"
#include "File.h"
#include "Log.h"
#ifdef _WINDOWS
#include "FbxLoader.h"
#endif

const uint32 DATA_VERSION = 1;

revModelLoader::revModelLoader()
{

}

revModelLoader::~revModelLoader()
{

}

bool revModelLoader::LoadFromFile(const char* path, revModel* model)
{
	char metaPath[256];
	revResource::MakeMetaPath(metaPath, path);
	if (!LoadMetaFile(metaPath, model)) {
		NATIVE_LOGE("Meta file not found : %s", metaPath);
		return false;
	}

	File file;
	if (!file.Open(path, FileMode::ReadBinary)) {
#ifdef _WINDOWS
		if (!LoadFBXFromFile(path, model)) return false;
		WriteMDL(path, *model);		
		return true;
#endif
		return false;
	}

	uint32 offset = 0;
	uint32 dataVer;
	offset = file.ReadData(&dataVer, sizeof(uint32), offset);
	if (dataVer != DATA_VERSION) {
#ifdef _WINDOWS
		if (!LoadFBXFromFile(path, model)) return false;
		WriteMDL(path, *model);
		return true;
#endif
		return false;
	}

	// readonly mesh? 
	bool readOnly;
	offset = file.ReadData(&readOnly, sizeof(bool), offset);

	// read transofrms 
	uint32 transformCount;
	offset = file.ReadData(&transformCount, sizeof(uint32), offset);
	for (uint32 i = 0; i < transformCount; ++i) {
		revVector3 scale;
		offset = file.ReadData(&scale, sizeof(revVector3), offset);
		revQuaternion rotation;
		offset = file.ReadData(&rotation, sizeof(revQuaternion), offset);
		revVector3 position;
		offset = file.ReadData(&position, sizeof(revVector3), offset);
		int32 parentIndex;
		offset = file.ReadData(&parentIndex, sizeof(int32), offset);
		
		revTransform* parent = nullptr;
		if (parentIndex != -1) parent = model->GetTransforms()[parentIndex];
		revTransform* transform = new revTransform(parent);
		transform->SetScale(scale);
		transform->SetRotation(rotation);
		transform->SetPosition(position);
		model->AddTransform(transform);
	}

	// read mesh 
	uint32 meshCount;
	offset = file.ReadData(&meshCount, sizeof(uint32), offset);
	for (uint32 i = 0; i < meshCount; ++i) {
		revMesh* mesh = new revMesh();
		uint32 nameLength;
		offset = file.ReadData(&nameLength, sizeof(uint32), offset);
		revString name;
		name.resize(nameLength);
		offset = file.ReadData(&name[0], nameLength, offset);
		mesh->SetName(name);
		offset = file.ReadData(&mesh->format, sizeof(uint32), offset);

		// read transform index
		offset = file.ReadData(&mesh->influenceTransformIndex, sizeof(int32), offset);

		// read vertex data
		if (readOnly) {
			uint32 count;
			offset = file.ReadData(&count, sizeof(uint32), offset);
			mesh->vertexData.resize(count);
			offset = file.ReadData(&mesh->vertexData[0], count * sizeof(f32), offset);
		}
		else {

		}
		// read index data
		uint32 indexCount;
		offset = file.ReadData(&indexCount, sizeof(uint32), offset);
		mesh->indices.resize(indexCount);
		offset = file.ReadData(&mesh->indices[0], sizeof(revIndex3) * indexCount, offset);

		model->meshes.push_back(mesh);
	}

	return true;
}

bool revModelLoader::LoadMetaFile(const char* metaPath, revModel* model)
{
#ifdef _DEBUG
	File metaFile;
	if (!metaFile.Open(metaPath, FileMode::ReadText)) {
		revSerializer::Serialize(metaPath, *model);
		return true;
	}
	else metaFile.Close();
#endif
	return revSerializer::Deserialize(metaPath, *model);
}

#ifdef _WINDOWS

bool revModelLoader::LoadFBXFromFile(const char* path, revModel* model)
{
	// try to load fbx and convert
	FBXLoader fbxLoader;
	revString fbxPath(path);
	uint32 pos = static_cast<uint32>(fbxPath.find_last_of('.'));
	fbxPath.replace(pos, fbxPath.size() - pos, ".fbx");
	if (!fbxLoader.LoadFromFile(fbxPath, model)) return false;
	return true;
}


void revModelLoader::WriteMDL(const char* path, const revModel& model)
{
	DeleteFileA(path);

	File file;
	file.Open(path, FileMode::WriteBinary);
	uint32 fileOffset = 0;
	const auto& meshes = model.GetMeshes();

	// write data ver
	file.WriteAppend(&DATA_VERSION, sizeof(uint32));

	// write readonly flag 
	bool readOnly = model.GetImportSetting().IsReadOnly();
	file.WriteAppend(&readOnly, sizeof(bool));

	// write transform info 
	uint32 transformCount = static_cast<uint32>(model.transforms.size());
	file.WriteAppend(&transformCount, sizeof(uint32));
	for (uint32 i = 0; i < transformCount; ++i) {
		revVector3 scale = model.transforms[i]->GetScale();
		file.WriteAppend(&scale, sizeof(revVector3));
		revQuaternion rotation = model.transforms[i]->GetRotaion();
		file.WriteAppend(&rotation, sizeof(revQuaternion));
		revVector3 position = model.transforms[i]->GetPosition();
		file.WriteAppend(&position, sizeof(revVector3));
		
		int32 parentIndex = -1;
		revTransform* parent = model.transforms[i]->GetParent();
		if (parent != nullptr) {
			for (uint32 j = 0; j < i; ++j) {
				if (parent == model.transforms[j]) {
					parentIndex = j;
					break;
				}
			}
		}
		file.WriteAppend(&parentIndex, sizeof(int32));
	}

	uint32 meshCount = static_cast<uint32>(meshes.size());
	file.WriteAppend(&meshCount, sizeof(uint32));
	for (uint32 i = 0; i < meshCount; ++i) {
		uint32 nameLength = static_cast<uint32>(meshes[i]->GetName().size());
		file.WriteAppend(&nameLength, sizeof(uint32));
		file.WriteAppend(meshes[i]->GetName().data(), nameLength);
		file.WriteAppend(&meshes[i]->format, sizeof(meshes[i]->format));
		file.WriteAppend(&meshes[i]->influenceTransformIndex, sizeof(int32));

		// write vertex data
		uint32 vertexDataCount = static_cast<uint32>(meshes[i]->vertexData.size());
		file.WriteAppend(&vertexDataCount, sizeof(uint32));
		file.WriteAppend(meshes[i]->vertexData.data(), vertexDataCount * sizeof(f32));
		
		// read write data needs each element
		if (!readOnly) {
			if (meshes[i]->hasElement(INPUT_ELEMENT_TYPE::POSITION)) {
				uint32 count = static_cast<uint32>(meshes[i]->vertices.size());
				file.WriteAppend(&count, sizeof(uint32));
				file.WriteAppend(meshes[i]->vertices.data(), sizeof(revVector3) * count);
			}
			if (meshes[i]->hasElement(INPUT_ELEMENT_TYPE::NORMAL)) {
				uint32 count = static_cast<uint32>(meshes[i]->normals.size());
				file.WriteAppend(&count, sizeof(uint32));
				file.WriteAppend(meshes[i]->normals.data(), sizeof(revVector3) * count);
			}
			if (meshes[i]->hasElement(INPUT_ELEMENT_TYPE::TANGENT)) {
				// TODO:
				//uint32 count = static_cast<uint32>(meshes[i]->tangents.size());
				//file.WriteAppend(&count, sizeof(uint32));
				//file.WriteAppend(meshes[i]->normals.data(), sizeof(revVector3) * count);
			}
			if (meshes[i]->hasElement(INPUT_ELEMENT_TYPE::TEXCOORD0)) {
				uint32 count = static_cast<uint32>(meshes[i]->texCoords[0].size());
				file.WriteAppend(&count, sizeof(uint32));
				file.WriteAppend(meshes[i]->texCoords[0].data(), sizeof(revVector2) * count);
			}
			if (meshes[i]->hasElement(INPUT_ELEMENT_TYPE::TEXCOORD1)) {
				uint32 count = static_cast<uint32>(meshes[i]->texCoords[1].size());
				file.WriteAppend(&count, sizeof(uint32));
				file.WriteAppend(meshes[i]->texCoords[1].data(), sizeof(revVector2) * count);
			}
			if (meshes[i]->hasElement(INPUT_ELEMENT_TYPE::TEXCOORD0)) {
				uint32 count = static_cast<uint32>(meshes[i]->texCoords[2].size());
				file.WriteAppend(&count, sizeof(uint32));
				file.WriteAppend(meshes[i]->texCoords[2].data(), sizeof(revVector2) * count);
			}
			if (meshes[i]->hasElement(INPUT_ELEMENT_TYPE::TEXCOORD0)) {
				uint32 count = static_cast<uint32>(meshes[i]->texCoords[3].size());
				file.WriteAppend(&count, sizeof(uint32));
				file.WriteAppend(meshes[i]->texCoords[3].data(), sizeof(revVector2) * count);
			}

			if (meshes[i]->hasElement(INPUT_ELEMENT_TYPE::COLOR0)) {
				uint32 count = static_cast<uint32>(meshes[i]->colors[0].size());
				file.WriteAppend(&count, sizeof(uint32));
				file.WriteAppend(meshes[i]->colors[0].data(), sizeof(revColor) * count);
			}
			if (meshes[i]->hasElement(INPUT_ELEMENT_TYPE::COLOR1)) {
				uint32 count = static_cast<uint32>(meshes[i]->colors[1].size());
				file.WriteAppend(&count, sizeof(uint32));
				file.WriteAppend(meshes[i]->colors[1].data(), sizeof(revColor) * count);
			}
			if (meshes[i]->hasElement(INPUT_ELEMENT_TYPE::COLOR2)) {
				uint32 count = static_cast<uint32>(meshes[i]->colors[2].size());
				file.WriteAppend(&count, sizeof(uint32));
				file.WriteAppend(meshes[i]->colors[2].data(), sizeof(revColor) * count);
			}
			if (meshes[i]->hasElement(INPUT_ELEMENT_TYPE::COLOR3)) {
				uint32 count = static_cast<uint32>(meshes[i]->colors[3].size());
				file.WriteAppend(&count, sizeof(uint32));
				file.WriteAppend(meshes[i]->colors[3].data(), sizeof(revColor) * count);
			}
		}

		uint32 indexCount = static_cast<uint32>(meshes[i]->indices.size());
		file.WriteAppend(&indexCount, sizeof(uint32));
		file.WriteAppend(meshes[i]->indices.data(), sizeof(revIndex3) * indexCount);
		

	}

	file.Close();
}

#endif