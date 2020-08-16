#ifndef __DX12VERTEXBUFFERVIEW_H__
#define __DX12VERTEXBUFFERVIEW_H__
#ifdef _USE_DIRECTX12

#include "revGraphicsResource.h"
#include "DX12DescriptorHeap.h"
#include "DX12Buffer.h"

class DX12VertexBufferView : public revGraphicsResource
{
public:
	DX12VertexBufferView(revDevice* device) : revGraphicsResource(device) {}
	virtual ~DX12VertexBufferView(){}
	
	void Create(const revGraphicsBuffer* buffer);
	virtual void Destroy();

	const D3D12_VERTEX_BUFFER_VIEW* GetResourceView() const { return &view; }

private:
	D3D12_VERTEX_BUFFER_VIEW view;
};

#endif
#endif