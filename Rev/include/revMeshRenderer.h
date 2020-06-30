#ifndef __REVMESHRENDERER_H__
#define __REVMESHRENDERER_H__

#include "revModel.h"
#include "revMesh.h"
#include "revMaterial.h"
#include "revGraphicsBuffer.h"
#include "revGraphicsCommandList.h"

class revMeshRenderer
{
public:
	revMeshRenderer();
	~revMeshRenderer();

	void SetModel(const revModel& model);

	void SetMeshes(const revArray<revMesh>& meshes);
	void SetMesh(uint32 index, const revMesh& mesh);

	void SetMaterials(const revArray<revMaterial*>& materials);
	void SetMaterial(uint32 index, revMaterial* material);

	virtual void Draw(revGraphicsCommandList& commandList) = 0;

protected:
	revArray<revGraphicsBuffer*> vertexBuffers;
	revArray<revGraphicsBuffer*> indexBuffers;
	revArray<revMaterial*> materials;

};

#endif
