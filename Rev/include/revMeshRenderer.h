#ifndef __REVMESHRENDERER_H__
#define __REVMESHRENDERER_H__

#include "revModel.h"
#include "revMesh.h"
#include "revMaterial.h"
#include "revCamera.h"
#include "revGraphicsBuffer.h"
#include "revGraphicsCommandList.h"

class revMeshRenderer
{
public:
	revMeshRenderer();
	virtual ~revMeshRenderer();

	virtual void SetModel(const revModel* model);

	virtual void SetMeshes(const revArray<revMesh*>& meshes);
	virtual void SetMesh(uint32 index, const revMesh* mesh);

	virtual void SetMaterialToAllSubMesh(revMaterial* material);
	virtual void SetMaterials(const revArray<revMaterial*>& materials);
	virtual void SetMaterial(uint32 index, revMaterial* material);

	virtual void Update(const revMatrix44& world = revMatrix44::IDENTITY);
	virtual void PrepareDraw(const revCamera& camera) = 0;
	virtual void Destroy();
protected:
	const revModel* model;
	revArray<revTransform*> transforms;
	revArray<revGraphicsBuffer*> transformConstantBuffers;
	revArray<revMaterial*> materials;
	revArray<revGraphicsBuffer*> materialsConstantBuffers;
	static const int32 DONT_HAVE_CONSTANT_BUFFER = -1;
	struct DrawResources {
		DrawResources() :
			vertexBuffer(nullptr),
			indexBuffer(nullptr),
			transformIndex(DONT_HAVE_CONSTANT_BUFFER),
			materialIndex(DONT_HAVE_CONSTANT_BUFFER)
		{
		}

		revGraphicsBuffer* vertexBuffer;
		revGraphicsBuffer* indexBuffer;
		int32 transformIndex;
		int32 materialIndex;
	};
	revArray<DrawResources*> drawResources;
};

#endif
