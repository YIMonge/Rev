#ifndef __DX12CONSTANTBUFFERVIEW_H__
#define __DX12CONSTANTBUFFERVIEW_H__
#ifdef _USE_DIRECTX12

#include "revShaderResourceView.h"
#include "DX12DescriptorHeap.h"
#include "DX12Buffer.h"

class DX12ConstantBufferView : public revShaderResourceView
{
public:
	DX12ConstantBufferView() {}
	virtual ~DX12ConstantBufferView(){}
	
	void Create(revDevice* device, const revGraphicsBuffer* cbuffer, const D3D12_CPU_DESCRIPTOR_HANDLE& heap);
	virtual void Destroy();
private:
	
};

#endif
#endif
