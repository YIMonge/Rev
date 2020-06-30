#ifndef __DX12VERTEXBUFFERVIEW_H__
#define __DX12VERTEXBUFFERVIEW_H__
#ifdef _USE_DIRECTX12

#include "revShaderResourceView.h"
#include "DX12DescriptorHeap.h"
#include "DX12Buffer.h"

class DX12VertexBufferView : public revShaderResourceView
{
public:
	DX12VertexBufferView() {}
	virtual ~DX12VertexBufferView(){}
	
	void Create(revDevice* device, const revGraphicsBuffer* buffer);
	virtual void Destroy();

	const D3D12_VERTEX_BUFFER_VIEW* GetResourceView() const { return &view; }

private:
	D3D12_VERTEX_BUFFER_VIEW view;
};

#endif
#endif