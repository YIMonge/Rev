#include "FBXLoader.h"
#include "Log.h"
#ifdef _DEBUG

using namespace fbxsdk;

enum FBX_CONTROL_PONIT_FORMAT_FLAG
{
	FBX_FORMAT_FLAG_VERTEX = 0x01,
	FBX_FORMAT_FLAG_NORMAL = 0x02,
	FBX_FORMAT_FLAG_BINORMAL = 0x04,
	FBX_FORMAT_FLAG_TANGENT = 0x08,
	FBX_FORMAT_FLAG_TEXCOORD = 0x10,
	FBX_FORMAT_FLAG_COLOR = 0x20,
};

struct ControlPoint
{
	revVector3 position;
	revVector3 normal;
	revVector2 texCoord;
	revVector3 biNormal;
	revColor color;
};

FBXLoader::FBXLoader():
	manager(nullptr),
	scene(nullptr)
{
	manager = FbxManager::Create();
}

FBXLoader::~FBXLoader()
{
	if (manager != nullptr) {
		manager->Destroy();
	}
}

bool FBXLoader::LoadFromFile(const revString& path, revModel* model)
{
	if (model == nullptr) return false;

	FbxImporter* importer = FbxImporter::Create(manager, "");
	if (!importer->Initialize(path.c_str(), -1, manager->GetIOSettings())) {
		NATIVE_LOGE("[FBX] Load Error %s, %d", __FILE__, __LINE__);
		return false;
	}
	
	scene = FbxScene::Create(manager, "scence");
	importer->Import(scene);

	FbxSystemUnit SceneSystemUnit = scene->GetGlobalSettings().GetSystemUnit();
	if (SceneSystemUnit.GetScaleFactor() != 1.0)
	{
		FbxSystemUnit::m.ConvertScene(scene);
	}

	FbxGeometryConverter converter(manager);
	converter.Triangulate(scene, true);
	FbxNode* root = scene->GetRootNode();
	if (root) {
		ImportNode(root, model, nullptr);
	}

	scene->Destroy();
	importer->Destroy();
	return true;
}

void FBXLoader::ImportNode(fbxsdk::FbxNode* node, revModel* model, revTransform* parent)
{
	revTransform* transform = ImportMatrix(node, model, parent);
	ImportVertexData(node, model, static_cast<int32>(model->GetTransforms().size()) - 1);

	const uint32 materialCount = node->GetMaterialCount();
	for (uint32 i = 0; i < materialCount; ++i) {
		FbxSurfaceMaterial* material = node->GetMaterial(i);
		ImportMaterialData(material, model);		
	}

	uint32 childNodeCount = node->GetChildCount();
	for (uint32 i = 0; i < childNodeCount; ++i) {
		ImportNode(node->GetChild(i), model, transform);
	}
}



void FBXLoader::ImportVertexData(FbxNode* node, revModel* model, int32 influenceTransformIndex)
{
	if (node == nullptr) return;

	revMesh* meshResource = new revMesh();
	meshResource->SetName(node->GetName());

	FbxMesh* meshNode = node->GetMesh();
	if (meshNode != nullptr) {
		meshResource->SetTransformIndex(influenceTransformIndex);

		const uint32 vertexCount = meshNode->GetControlPointsCount();
		const uint32 polygonCount = meshNode->GetPolygonCount();

		revArray<revVector3> vertices;
		revArray<revVector3> normals;
		revArray<revVector2> texcoords[MAX_TEXCOORD_NUM];
		revArray<revColor> vertexColors[MAX_VERTEX_COLOR_NUM];
		revArray<revIndex3> indexies;

		uint32 normalCount = meshNode->GetElementNormalCount();
		vertices.reserve(vertexCount);
		normals.reserve(vertexCount);
		indexies.reserve(polygonCount * 3);

		// index
		for (uint32 i = 0; i < polygonCount; ++i) {
			// polygon size must be 3 cause Triangulate. 
			revIndex3 index;
			for (uint32 j = 0; j < 3; ++j) {
				uint32 fbxIndex = meshNode->GetPolygonVertex(i, j);
				FbxVector4 fbxPosition = meshNode->GetControlPointAt(fbxIndex);
				revVector3 position(fbxPosition[0], fbxPosition[1], fbxPosition[2]);
				vertices.push_back(position);
				
				if (normalCount > 0) {
					FbxVector4 fbxNormal;
					meshNode->GetPolygonVertexNormal(i, j, fbxNormal);
					revVector3 normal(fbxNormal[0], fbxNormal[1], fbxNormal[2]);
					normals.push_back(normal);
				}
				index.data[j] = i * 3 + j;
			}
			
			indexies.push_back(index);
		}

		if (polygonCount > 0) {
			meshResource->SetVertexArray(vertices);
			meshResource->SetNormalArray(normals);
			meshResource->SetIndexArray(indexies);

			meshResource->SetFormat(INPUT_ELEMENT_TYPE::POSITION);
			if(normalCount > 0) meshResource->SetFormat(INPUT_ELEMENT_TYPE::NORMAL);
		}

		// uv
		FbxStringList uvSetNames;
		meshNode->GetUVSetNames(uvSetNames);
		uint32 uvSetCount = uvSetNames.GetCount();
		bool unmapped = false;


		INPUT_ELEMENT_TYPE TEXCOORD_ELEMENT[] = {
			INPUT_ELEMENT_TYPE::TEXCOORD0,
			INPUT_ELEMENT_TYPE::TEXCOORD1,
			INPUT_ELEMENT_TYPE::TEXCOORD2,
			INPUT_ELEMENT_TYPE::TEXCOORD3,
		};

		for (uint32 i = 0; i < uvSetCount; ++i) {
			FbxString uvSetName = uvSetNames.GetStringAt(i);
			texcoords[i].reserve(polygonCount * 3);
			for (uint32 j = 0; j < polygonCount; ++j) {
				// polygon size must be 3 cause Triangulate. 
				for (uint32 k = 0; k < 3; ++k) {
					FbxVector2 fbxUV;
					meshNode->GetPolygonVertexUV(j, k, uvSetName, fbxUV, unmapped);
					revVector2 uv(fbxUV[0], fbxUV[1]);
					texcoords[i].push_back(uv);
				}
			}
			meshResource->SetTexCoordArray(i, texcoords[i]);
			meshResource->SetFormat(TEXCOORD_ELEMENT[i]);
		}

		INPUT_ELEMENT_TYPE COLOR_ELEMENT[] = {
			INPUT_ELEMENT_TYPE::COLOR0,
			INPUT_ELEMENT_TYPE::COLOR1,
			INPUT_ELEMENT_TYPE::COLOR2,
			INPUT_ELEMENT_TYPE::COLOR3,
		};

		// color 
		uint32 elementColorCount = meshNode->GetElementVertexColorCount();
		for (uint32 i = 0; i < elementColorCount; ++i) {
			FbxGeometryElementVertexColor* colorBuffer = meshNode->GetElementVertexColor(i);

			auto& colors = colorBuffer->GetDirectArray();
			auto& colorIndexies = colorBuffer->GetIndexArray();

			uint32 colorIndexCount = colorIndexies.GetCount();
			vertexColors[i].resize(colorIndexCount);

			for (uint32 j = 0; j < colorIndexCount; ++j) {
				uint32 index = colorIndexies.GetAt(j);
				FbxColor fbxColor = colors.GetAt(index);
				revColor color(static_cast<f32>(fbxColor.mRed), 
					static_cast<f32>(fbxColor.mGreen), 
					static_cast<f32>(fbxColor.mBlue), 
					static_cast<f32>(fbxColor.mAlpha));
				vertexColors[i].push_back(color);
			}
			meshResource->SetColorArray(i, vertexColors[i]);
			meshResource->SetFormat(COLOR_ELEMENT[i]);
		}

		meshResource->CreateVertexBufferData();
		model->AddMesh(meshResource);
	}
}

revTransform* FBXLoader::ImportMatrix(FbxNode* node, revModel* model, revTransform* parent)
{
	if (node == nullptr) return nullptr;
	FbxAnimEvaluator* animEvaluator = scene->GetAnimationEvaluator();
	revTransform* transform = new revTransform(parent);
	
	auto fbxGlobalTransform = animEvaluator->GetNodeGlobalTransform(node);
	FbxVector4 fbxScale = fbxGlobalTransform.GetS();
	FbxVector4 fbxRotation = fbxGlobalTransform.GetR();
	FbxVector4 fbxPosition = fbxGlobalTransform.GetT();
	//FbxVector4 fbxScale = animEvaluator->GetNodeLocalScaling(node);
	//FbxVector4 fbxRotation = animEvaluator->GetNodeLocalRotation(node);
	//FbxVector4 fbxPosition = animEvaluator->GetNodeLocalTranslation(node);

	revVector3 scale(static_cast<f32>(fbxScale[0]), static_cast<f32>(fbxScale[1]), static_cast<f32>(fbxScale[2]));
	revQuaternion rotation(revVector3(
		MathUtil::ToRadian(static_cast<f32>(fbxRotation[0])),
		MathUtil::ToRadian(static_cast<f32>(fbxRotation[1])),
		MathUtil::ToRadian(static_cast<f32>(fbxRotation[2]))));
	revVector3 position(static_cast<f32>(fbxPosition[0]), static_cast<f32>(fbxPosition[1]), static_cast<f32>(fbxPosition[2]));

	transform->SetScale(scale);
	transform->SetRotation(rotation);
	transform->SetPosition(position);

	model->AddTransform(transform);
	return transform;
}

void FBXLoader::ImportMaterialData(FbxSurfaceMaterial* material, revModel* model)
{
	if (material == nullptr) return;

	if (material->GetClassId().Is(FbxSurfaceLambert::ClassId)) {

	}
	else if (material->GetClassId().Is(FbxSurfacePhong::ClassId)) {

	}


	//FbxDouble3 emissive = Ge

}

FbxDouble3 FBXLoader::ImportMaterialProperty(FbxSurfaceMaterial* material, const char* propertyName, const char* factorName)
{
	FbxDouble3 result;
	FbxProperty property = material->FindProperty(propertyName);
	FbxProperty factor = material->FindProperty(factorName);

	if (property.IsValid() && factor.IsValid()) {
		result = property.Get<FbxDouble3>();
		f64 factorVal = factor.Get<FbxDouble>();
		result[0] *= factorVal;
		result[1] *= factorVal;
		result[2] *= factorVal;


	}
	return result;
}

#endif