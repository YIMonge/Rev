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
	if (!revResourceLoader::LoadFromFile(path, model)) return false;

	uint32 offset = 0;
	uint32 dataVer;
	offset = file.ReadData(&dataVer, sizeof(uint32), offset);
	if (dataVer != DATA_VERSION) {
		file.Close();
#ifdef _WINDOWS
		if (!LoadFBXFromFile(resourcePath.c_str(), model)) return false;
		WriteMDL(resourcePath.c_str(), *model);
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

	file.Close();
	return true;
}

bool revModelLoader::OnLoadFailed(const char* resourcePath, revModel* model)
{
#ifdef _WINDOWS
	if (!LoadFBXFromFile(resourcePath, model)) return false;
	WriteMDL(resourcePath, *model);
	return true;
#endif
	return false;
}


#ifdef _WINDOWS

bool revModelLoader::LoadFBXFromFile(const char* path, revModel* model)
{
	if (model == nullptr) return false;
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

	File writingFile;
	writingFile.Open(path, FileMode::WriteBinary);
	uint32 fileOffset = 0;
	const auto& meshes = model.GetMeshes();

	// write data ver
	writingFile.WriteAppend(&DATA_VERSION, sizeof(uint32));

	// write readonly flag 
	bool readOnly = model.GetImportSetting().IsReadOnly();
	writingFile.WriteAppend(&readOnly, sizeof(bool));

	// write transform info 
	uint32 transformCount = static_cast<uint32>(model.transforms.size());
	writingFile.WriteAppend(&transformCount, sizeof(uint32));
	for (uint32 i = 0; i < transformCount; ++i) {
		revVector3 scale = model.transforms[i]->GetScale();
		writingFile.WriteAppend(&scale, sizeof(revVector3));
		revQuaternion rotation = model.transforms[i]->GetRotaion();
		writingFile.WriteAppend(&rotation, sizeof(revQuaternion));
		revVector3 position = model.transforms[i]->GetPosition();
		writingFile.WriteAppend(&position, sizeof(revVector3));
		
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
		writingFile.WriteAppend(&parentIndex, sizeof(int32));
	}

	uint32 meshCount = static_cast<uint32>(meshes.size());
	writingFile.WriteAppend(&meshCount, sizeof(uint32));
	for (uint32 i = 0; i < meshCount; ++i) {
		uint32 nameLength = static_cast<uint32>(meshes[i]->GetName().size());
		writingFile.WriteAppend(&nameLength, sizeof(uint32));
		writingFile.WriteAppend(meshes[i]->GetName().data(), nameLength);
		writingFile.WriteAppend(&meshes[i]->format, sizeof(meshes[i]->format));
		writingFile.WriteAppend(&meshes[i]->influenceTransformIndex, sizeof(int32));

		// write vertex data
		uint32 vertexDataCount = static_cast<uint32>(meshes[i]->vertexData.size());
		writingFile.WriteAppend(&vertexDataCount, sizeof(uint32));
		writingFile.WriteAppend(meshes[i]->vertexData.data(), vertexDataCount * sizeof(f32));
		
		// read write data needs each element
		if (!readOnly) {
			if (meshes[i]->hasElement(INPUT_ELEMENT_TYPE::POSITION)) {
				uint32 count = static_cast<uint32>(meshes[i]->vertices.size());
				writingFile.WriteAppend(&count, sizeof(uint32));
				writingFile.WriteAppend(meshes[i]->vertices.data(), sizeof(revVector3) * count);
			}
			if (meshes[i]->hasElement(INPUT_ELEMENT_TYPE::NORMAL)) {
				uint32 count = static_cast<uint32>(meshes[i]->normals.size());
				writingFile.WriteAppend(&count, sizeof(uint32));
				writingFile.WriteAppend(meshes[i]->normals.data(), sizeof(revVector3) * count);
			}
			if (meshes[i]->hasElement(INPUT_ELEMENT_TYPE::TANGENT)) {
				// TODO:
				//uint32 count = static_cast<uint32>(meshes[i]->tangents.size());
				//writingFile.WriteAppend(&count, sizeof(uint32));
				//writingFile.WriteAppend(meshes[i]->normals.data(), sizeof(revVector3) * count);
			}
			if (meshes[i]->hasElement(INPUT_ELEMENT_TYPE::TEXCOORD0)) {
				uint32 count = static_cast<uint32>(meshes[i]->texCoords[0].size());
				writingFile.WriteAppend(&count, sizeof(uint32));
				writingFile.WriteAppend(meshes[i]->texCoords[0].data(), sizeof(revVector2) * count);
			}
			if (meshes[i]->hasElement(INPUT_ELEMENT_TYPE::TEXCOORD1)) {
				uint32 count = static_cast<uint32>(meshes[i]->texCoords[1].size());
				writingFile.WriteAppend(&count, sizeof(uint32));
				writingFile.WriteAppend(meshes[i]->texCoords[1].data(), sizeof(revVector2) * count);
			}
			if (meshes[i]->hasElement(INPUT_ELEMENT_TYPE::TEXCOORD0)) {
				uint32 count = static_cast<uint32>(meshes[i]->texCoords[2].size());
				writingFile.WriteAppend(&count, sizeof(uint32));
				writingFile.WriteAppend(meshes[i]->texCoords[2].data(), sizeof(revVector2) * count);
			}
			if (meshes[i]->hasElement(INPUT_ELEMENT_TYPE::TEXCOORD0)) {
				uint32 count = static_cast<uint32>(meshes[i]->texCoords[3].size());
				writingFile.WriteAppend(&count, sizeof(uint32));
				writingFile.WriteAppend(meshes[i]->texCoords[3].data(), sizeof(revVector2) * count);
			}

			if (meshes[i]->hasElement(INPUT_ELEMENT_TYPE::COLOR0)) {
				uint32 count = static_cast<uint32>(meshes[i]->colors[0].size());
				writingFile.WriteAppend(&count, sizeof(uint32));
				writingFile.WriteAppend(meshes[i]->colors[0].data(), sizeof(revColor) * count);
			}
			if (meshes[i]->hasElement(INPUT_ELEMENT_TYPE::COLOR1)) {
				uint32 count = static_cast<uint32>(meshes[i]->colors[1].size());
				writingFile.WriteAppend(&count, sizeof(uint32));
				writingFile.WriteAppend(meshes[i]->colors[1].data(), sizeof(revColor) * count);
			}
			if (meshes[i]->hasElement(INPUT_ELEMENT_TYPE::COLOR2)) {
				uint32 count = static_cast<uint32>(meshes[i]->colors[2].size());
				writingFile.WriteAppend(&count, sizeof(uint32));
				writingFile.WriteAppend(meshes[i]->colors[2].data(), sizeof(revColor) * count);
			}
			if (meshes[i]->hasElement(INPUT_ELEMENT_TYPE::COLOR3)) {
				uint32 count = static_cast<uint32>(meshes[i]->colors[3].size());
				writingFile.WriteAppend(&count, sizeof(uint32));
				writingFile.WriteAppend(meshes[i]->colors[3].data(), sizeof(revColor) * count);
			}
		}

		uint32 indexCount = static_cast<uint32>(meshes[i]->indices.size());
		writingFile.WriteAppend(&indexCount, sizeof(uint32));
		writingFile.WriteAppend(meshes[i]->indices.data(), sizeof(revIndex3) * indexCount);
		

	}

	writingFile.Close();
}

#endif