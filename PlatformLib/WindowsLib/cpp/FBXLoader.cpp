#include "FBXLoader.h"
#include "Log.h"
#include <set>

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
	manager(nullptr)
{
	manager = FbxManager::Create();
}

FBXLoader::~FBXLoader()
{
	if (manager != nullptr) {
		manager->Destroy();
	}
}

void FBXLoader::LoadFromFile(const revString& path, revMesh* mesh)
{
	FbxImporter* importer = FbxImporter::Create(manager, "");
	if (!importer->Initialize(path.c_str(), -1, manager->GetIOSettings())) {
		NATIVE_LOGE("[FBX] Load Error %s, %d", __FILE__, __LINE__);
		return;
	}
	FbxManager::GetFileFormatVersion(sdkMajorVersion, sdkMinorVersion, sdkRevision);
	
	// TODO: blender
	//FbxIOFileHeaderInfo* fileHeaderInfo = importer->GetFileHeaderInfo();
	
	
	FbxScene* scene = FbxScene::Create(manager, "scence");
	importer->Import(scene);
	FbxGeometryConverter converter(manager);
	converter.Triangulate(scene, true);

	revModel model;
	FbxNode* root = scene->GetRootNode();
	if (root) {
		ImportNode(root, &model);
	}

	scene->Destroy();
	importer->Destroy();
}

void FBXLoader::ImportNode(FbxNode* node, revModel* model)
{
	if (node == nullptr) return;

	revMesh meshResource;
	meshResource.SetName(node->GetName());

	FbxMesh* meshNode = node->GetMesh();
	if (meshNode != nullptr) {
		const uint32 vertexCount = meshNode->GetControlPointsCount();
		const uint32 polygonCount = meshNode->GetPolygonCount();

		revArray<revVector3> vertices;
		revArray<revVector3> normals;
		revArray<revVector2> texcoords[MAX_TEXCOORD_NUM];
		revArray<revIndex3> indexies;

		uint32 normalCount = meshNode->GetElementNormalCount();
		vertices.reserve(vertexCount);
		normals.reserve(vertexCount);
		indexies.reserve(polygonCount * 3);

		// vertex and normal
		for (uint32 i = 0; i < polygonCount; ++i) {
			// polygon size must be 3 cause Triangulate. 
			revIndex3 index;
			for (uint32 j = 0; j < 3; ++j) {
				index.data[j] = meshNode->GetPolygonVertex(i, j);

				FbxVector4 fbxPosition = meshNode->GetControlPointAt(index.data[j]);				
				revVector3 position(fbxPosition[0], fbxPosition[1], fbxPosition[2]);
				vertices.push_back(position);
				
				if (normalCount > 0) {
					FbxVector4 fbxNormal;
					meshNode->GetPolygonVertexNormal(i, j, fbxNormal);
					revVector3 normal(fbxNormal[0], fbxNormal[1], fbxNormal[2]);
					normals.push_back(normal);
				}
			}
			indexies.push_back(index);
		}

		if (polygonCount > 0) {
			meshResource.SetVertexArray(vertices);
			meshResource.SetNormalArray(normals);
			meshResource.SetIndexArray(indexies);

			meshResource.SetFormat(INPUT_ELEMENT_TYPE::POSITION);
			if(normalCount > 0) meshResource.SetFormat(INPUT_ELEMENT_TYPE::NORMAL);
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
			meshResource.SetTexCoordArray(i, texcoords[i]);
			meshResource.SetFormat(TEXCOORD_ELEMENT[i]);
		}

		INPUT_ELEMENT_TYPE COLOR_ELEMENT[] = {
			INPUT_ELEMENT_TYPE::COLOR0,
			INPUT_ELEMENT_TYPE::COLOR1,
			INPUT_ELEMENT_TYPE::COLOR2,
			INPUT_ELEMENT_TYPE::COLOR3,
		};
		// TODO: read color element

		meshResource.CreateVertexBufferData();
		model->AddMesh(meshResource);
	}

	uint32 childNodeCount = node->GetChildCount();
	for (uint32 i = 0; i < childNodeCount; ++i) {
		ImportNode(node->GetChild(i), model);
	}


}


void FBXLoader::LoadStaticMesh(FbxNode* parentNode)
{
	uint32 format;
	revArray<ControlPoint> controlPointArray;
	revArray<revIndex3> indexArray;
	FbxMesh* meshNode = parentNode->GetMesh();

	uint32 controlPointCount = meshNode->GetControlPointsCount();
	controlPointArray.reserve(controlPointCount);
	
	format |= FBX_FORMAT_FLAG_VERTEX;

	uint32 polygonCount = meshNode->GetPolygonCount();
	for (uint32 i = 0; i < polygonCount; ++i) {
		// always 3 because I did triangulate. 
		uint32 polygonSize = meshNode->GetPolygonSize(i);
		if (polygonSize != 3) NATIVE_LOGE("%s, %s", __FILE__, __LINE__);
		revIndex3 index;
		index.x = meshNode->GetPolygonVertex(i, 0);
		index.y	= meshNode->GetPolygonVertex(i, 1);
		index.z = meshNode->GetPolygonVertex(i, 2);
		indexArray.push_back(index);
	}

	for (uint32 i = 0; i < polygonCount; ++i) {
		for (uint32 j = 0; j < 3; ++j) {
			uint32 index = indexArray[i].data[j];
			auto fbxControlPoint = meshNode->GetControlPointAt(index);
			ControlPoint controlPoint;
			controlPoint.position.x = static_cast<float>(fbxControlPoint[0]);
			controlPoint.position.y = static_cast<float>(fbxControlPoint[1]);
			controlPoint.position.z = static_cast<float>(fbxControlPoint[2]);
			controlPoint.position.z = static_cast<float>(fbxControlPoint[2]);
			controlPointArray.push_back(controlPoint);		
		}
	}


	FbxGeometryElementNormal* elementNormal = meshNode->GetElementNormal(0);
	auto normalMappingMode = elementNormal->GetMappingMode();
	auto normalReferenceMode = elementNormal->GetReferenceMode();

	if (elementNormal->GetDirectArray().GetCount() > 0) {
		format |= FBX_FORMAT_FLAG_NORMAL;
	}

	switch (normalMappingMode)
	{
	case FbxLayerElement::eByPolygonVertex:
	{
		uint32 normalIndex = 0;
		for (uint32 i = 0; i < polygonCount; ++i) {
			for (uint32 j = 0; j < 3; ++j) {
				FbxVector4 n;
				meshNode->GetPolygonVertexNormal(i, j, n);
				controlPointArray[normalIndex++].normal = revVector3(n[0], n[1], n[2]);
			}
		}
	}
		break;


	case FbxLayerElement::eByControlPoint:
	{
		uint32 normalIndex = 0;
		for (uint32 i = 0; i < polygonCount; ++i) {
			for (uint32 j = 0; j < 3; ++j) {
				uint32 index = indexArray[i].data[j];
				auto n = elementNormal->GetDirectArray().GetAt(index);
				controlPointArray[normalIndex++].normal = revVector3(n[0], n[1], n[2]);
			}
		}
	}
		break;

	}

	// TODO: multi layer 
	FbxLayerElementUV* elementUV = meshNode->GetLayer(0)->GetUVs();
	auto uvMappingMode = elementNormal->GetMappingMode();
	auto uvReferenceMode = elementNormal->GetReferenceMode();

	const char* uvSetName = meshNode->GetLayer(0)->GetUVSets()[0]->GetName();

	uint32 uvCount = elementUV->GetDirectArray().GetCount();
	if (uvCount > 0) {
		format |= FBX_FORMAT_FLAG_TEXCOORD;
	}

	switch (uvMappingMode) {
	case FbxLayerElement::eByPolygonVertex:
		{
			uint32 uvIndex = 0;
			for (uint32 i = 0; i < polygonCount; ++i) {
				for (uint32 j = 0; j < 3; ++j) {
					FbxVector2 uv;
					bool mapped;
					meshNode->GetPolygonVertexUV(i, j, uvSetName, uv, mapped);
					controlPointArray[uvIndex++].texCoord = revVector2(uv[0], uv[1]);
				}
			}
		}
		break;
	}

	// load children 
	uint32 childCount = parentNode->GetChildCount();
	for (uint32 i = 0; i < childCount; ++i) {		
		LoadStaticMesh(parentNode->GetChild(i));
	}

}


#endif