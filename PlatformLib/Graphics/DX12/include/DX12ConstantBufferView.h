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
	
	void Create(const revGraphicsBuffer* cbuffer, revDescriptorHeap* descriptorHeap, revDescriptorHeap::Chunk* allocatedChunk = nullptr, uint32 offset = 0);
	virtual void Destroy();
private:
	revDescriptorHeap* descriptorHeap;
};

#endif
#endif
