#ifndef __DX12ROOTSIGNATURE_H__
#define __DX12ROOTSIGNATURE_H__

#include "DX12Device.h"
#include "revMaterial.h"
#include "revDescriptorBindingDesc.h"

class DX12RootSignature
{
public:
	DX12RootSignature() :
		sizeInBytes(0)

	{}
	virtual ~DX12RootSignature(){}
	
	bool Create(revDevice* device);
	bool Create(revDevice* device, const revDescriptorBindingDesc& desc);

	void Apply(revGraphicsCommandList& commandList, bool isGraphics = true);
	ID3D12RootSignature* Get() const { return rootSignature; }

private:
	revDevice*              device;        
	ID3D12RootSignature*    rootSignature;     
	uint32 sizeInBytes;
	revArray<uint32> byteOffsets;
};

#endif
