#ifndef __REVMESH_H__
#define __REVMESH_H__

#include "revArray.h"
#include "revMath.h"
#include "revResource.h"

class revMesh : public revResource
{
public:
	revMesh() {}
	~revMesh() {}

#ifdef  _DEBUG
	revArray<revVector3>& GetVertexArray() { return vertices; }
	revArray<revVector3>& GetNormalArray() { return normals; }
	revArray<revVector2>& GetTexCoordArray() { return texCoords; }
	revArray<revIndex3>& GetIndexArray() { return indices; }
#endif 

private:
	revArray<revVector3> vertices;
	revArray<revVector3> normals;
	revArray<revVector2> texCoords;
	revArray<revIndex3> indices;
	revMaterial material;
};

#endif