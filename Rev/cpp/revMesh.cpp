#include "revMesh.h"

void revMesh::CreateVertexBufferData()
{
	uint32 vertexCount = static_cast<uint32>(vertices.size());
	
	vertexData.clear();
	vertexData.reserve(GetSizeOfBytes() / sizeof(f32) * vertexCount);

	for (uint32 i = 0; i < vertexCount; ++i) {
		vertexData.push_back(vertices[i].x);
		vertexData.push_back(vertices[i].y);
		vertexData.push_back(vertices[i].z);

		if (hasElement(INPUT_ELEMENT_TYPE::NORMAL)) {
			vertexData.push_back(normals[i].x);
			vertexData.push_back(normals[i].y);
			vertexData.push_back(normals[i].z);
		}

		// TODO: 
		if (hasElement(INPUT_ELEMENT_TYPE::TANGENT)) {

		}

		// texcoord
		if (hasElement(INPUT_ELEMENT_TYPE::TEXCOORD0)) {
			vertexData.push_back(texCoords[0][i].x);
			vertexData.push_back(texCoords[0][i].y);
		}
		if (hasElement(INPUT_ELEMENT_TYPE::TEXCOORD1)) {
			vertexData.push_back(texCoords[1][i].x);
			vertexData.push_back(texCoords[1][i].y);
		}
		if (hasElement(INPUT_ELEMENT_TYPE::TEXCOORD2)) {
			vertexData.push_back(texCoords[2][i].x);
			vertexData.push_back(texCoords[2][i].y);
		}
		if (hasElement(INPUT_ELEMENT_TYPE::TEXCOORD3)) {
			vertexData.push_back(texCoords[3][i].x);
			vertexData.push_back(texCoords[3][i].y);
		}

		if (hasElement(INPUT_ELEMENT_TYPE::COLOR0)) {
			vertexData.push_back(colors[0][i].r);
			vertexData.push_back(colors[0][i].g);
			vertexData.push_back(colors[0][i].b);
			vertexData.push_back(colors[0][i].a);
		}
		if (hasElement(INPUT_ELEMENT_TYPE::COLOR1)) {
			vertexData.push_back(colors[1][i].r);
			vertexData.push_back(colors[1][i].g);
			vertexData.push_back(colors[1][i].b);
			vertexData.push_back(colors[1][i].a);
		}
		if (hasElement(INPUT_ELEMENT_TYPE::COLOR2)) {
			vertexData.push_back(colors[2][i].r);
			vertexData.push_back(colors[2][i].g);
			vertexData.push_back(colors[2][i].b);
			vertexData.push_back(colors[2][i].a);
		}
		if (hasElement(INPUT_ELEMENT_TYPE::COLOR3)) {
			vertexData.push_back(colors[3][i].r);
			vertexData.push_back(colors[3][i].g);
			vertexData.push_back(colors[3][i].b);
			vertexData.push_back(colors[3][i].a);
		}
	}

}
