#ifndef __REVMESH_H__
#define __REVMESH_H__

#include "revArray.h"
#include "revMath.h"
#include "revResource.h"
#include "revGraphicsTypedef.h"
class revMesh : public revResource
{
friend class revModelLoader;
public:
	revMesh() :
	readOnly(false){}
	~revMesh() {}

	const revArray<f32>& GetVertexData() const { return vertexData; }
	const revArray<revVector3>& GetVertexArray() const { return vertices; }
	const revArray<revVector3>& GetNormalArray() const { return normals; }
	const revArray<revVector2>& GetTexCoordArray(uint32 index) const { return texCoords[index]; }
	const revArray<revColor>& GetColorArray(uint32 index) const { return colors[index]; }
	const revArray<revIndex3>& GetIndexArray() const { return indices; }
	bool hasElement(INPUT_ELEMENT_TYPE element) const { return format & (1 << static_cast<uint32>(element));  }
	uint32 GetSizeOfBytes() const
	{
		uint32 sizeOfBytes = 0;
		for (uint32 i = 0; i < static_cast<uint32>(INPUT_ELEMENT_TYPE::MAX_NUM); ++i) {
			if (hasElement(static_cast<INPUT_ELEMENT_TYPE>(i))) sizeOfBytes += GRAPHICS_SEMANTICS[i].sizeOfBytes;
		}
		return sizeOfBytes;
	}

	void CreateVertexBufferData();

#ifdef  _DEBUG
	void SetVertexArray(const revArray<revVector3>& v) { vertices = v; }
	void SetNormalArray(const revArray<revVector3>& n) { normals = n; }
	void SetTexCoordArray(uint32 index, const revArray<revVector2>& uv) {texCoords[index] = uv; }
	void SetColorArray(uint32 index, const revArray<revColor>& c) { colors[index] = c; }
	void SetIndexArray(const revArray<revIndex3>& i) { indices = i; }
	void SetFormat(INPUT_ELEMENT_TYPE element) { format |= (1 << static_cast<uint32>(element)); }
	void SetReadOnly(bool flag) { readOnly = flag; }
#endif 

private:
	// if disable readonly you can access these data and recreate vertex data 
	revArray<revVector3> vertices;
	revArray<revVector3> normals;
	revArray<revVector2> texCoords[MAX_TEXCOORD_NUM];
	revArray<revColor>	 colors[MAX_VERTEX_COLOR_NUM];
	// data for vertex buffer 
	revArray<f32> vertexData;
	// data for index buffer 
	revArray<revIndex3> indices;
	uint32 materialId;
	uint32 format;
	bool readOnly;
};

#endif