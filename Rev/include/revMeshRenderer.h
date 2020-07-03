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
	virtual ~revMeshRenderer();

	void SetModel(const revModel& model);

	virtual void SetMeshes(const revArray<revMesh>& meshes);
	virtual void SetMesh(uint32 index, const revMesh& mesh);

	virtual void SetMaterials(const revArray<revMaterial*>& materials);
	virtual void SetMaterial(uint32 index, revMaterial* material);

	virtual void Draw(revGraphicsCommandList& commandList) = 0;

protected:
	revArray<revGraphicsBuffer*> vertexBuffers;
	revArray<revGraphicsBuffer*> indexBuffers;
	revArray<revMaterial*> materials;

};

#endif
