#ifndef __DX12MESHRENDERER_H__
#define __DX12MESHRENDERER_H__

#ifdef _USE_DIRECTX12

#include "revMeshRenderer.h"
#include "DX12VertexBufferView.h"
#include "DX12IndexBufferView.h"
#include "DX12ConstantBufferView.h"

class DX12MeshRenderer : public revMeshRenderer
{
public:
	DX12MeshRenderer();
	virtual ~DX12MeshRenderer();

	virtual void SetModel(const revModel* model);
	virtual void SetMesh(uint32 index, const revMesh* mesh);
	void Destroy();

	void Initialize(DX12DescriptorHeap* cBufferHeap);
	virtual void PrepareDraw(const revCamera& camera);
	virtual void Draw(revGraphicsCommandList& commandList, DX12DescriptorHeap& cBufferHeap, DX12DescriptorHeap& textureHeap, DX12DescriptorHeap& samplerHeap);

private:
	revArray<DX12VertexBufferView*> vertexBufferViews;
	revArray<DX12IndexBufferView*> indexBufferViews;
	revArray<DX12ConstantBufferView*> constantBufferViews;
	DX12DescriptorHeap::Chunk cbufferHeapChunk;
};


#endif
#endif
