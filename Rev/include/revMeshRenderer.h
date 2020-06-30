#ifndef __REVMESHRENDERER_H__
#define __REVMESHRENDERER_H__

#include "revMesh.h"
#include "revGraphicsBuffer.h"

class revMeshRenderer
{
public:
	revMeshRenderer();
	~revMeshRenderer();

	void SetMeshes(const revArray<revMesh>& meshes);
	void SetMesh(uint32 index, const revMesh& mesh);

private:
	revArray<revGraphicsBuffer*> vertexBuffers;
};

#endif
