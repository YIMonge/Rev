#ifndef __DX12INDEXBUFFERVIEW_H__
#define __DX12INDEXBUFFERVIEW_H__
#ifdef _USE_DIRECTX12

#include "revShaderResourceView.h"
#include "DX12DescriptorHeap.h"
#include "DX12Buffer.h"

class DX12IndexBufferView : public revShaderResourceView
{
public:
	DX12IndexBufferView() {}
	virtual ~DX12IndexBufferView(){}
	
	void Create(revDevice* device, const revGraphicsBuffer* buffer);
	virtual void Destroy();

	const D3D12_INDEX_BUFFER_VIEW* GetResourceView() const { return &view; }
	uint32 GetCount() const { return indexCount; }
private:
	D3D12_INDEX_BUFFER_VIEW view;
	uint32 indexCount;
};

#endif
#endif