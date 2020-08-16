#ifndef __DX12CONSTANTBUFFERVIEW_H__
#define __DX12CONSTANTBUFFERVIEW_H__
#ifdef _USE_DIRECTX12

#include "revGraphicsResource.h"
#include "DX12DescriptorHeap.h"
#include "DX12Buffer.h"

class DX12ConstantBufferView : public revGraphicsResource
{
public:
	DX12ConstantBufferView(revDevice* device) : revGraphicsResource(device) {}
	virtual ~DX12ConstantBufferView(){}
	
	void Create(const revGraphicsBuffer* cbuffer, const D3D12_CPU_DESCRIPTOR_HANDLE& heap);
	virtual void Destroy();
private:
	
};

#endif
#endif
