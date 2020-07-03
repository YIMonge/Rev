#ifndef __DX12MESHRENDERER_H__
#define __DX12MESHRENDERER_H__

#ifdef _USE_DIRECTX12

#include "revMeshRenderer.h"
#include "DX12VertexBufferView.h"
#include "DX12IndexBufferView.h"


class DX12MeshRenderer : public revMeshRenderer
{
public:
	DX12MeshRenderer();
	virtual ~DX12MeshRenderer();

	virtual void SetMesh(uint32 index, const revMesh& mesh);
	void Destroy();
	virtual void Draw(revGraphicsCommandList& commandList);

private:
	revArray<DX12VertexBufferView*> vertexBufferViews;
	revArray<DX12IndexBufferView*> indexBufferViews;
};


#endif
#endif
