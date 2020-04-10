#ifndef __DX12ROOTSIGNATURE_H__
#define __DX12ROOTSIGNATURE_H__

#include "DX12Device.h"


class DescriptorLayoutDesc
{
public:
	struct Range
	{
		DESCRIPTOR_TYPE type;
		uint32 registerIndex;
		uint32 count;
		uint32 space;
	};

private:
	revArray<Range> ranges;
};

class RootSignatureDesc
{

private:
	//revArray<
};

class DX12RootSignature
{
public:
	DX12RootSignature() {}
	virtual ~DX12RootSignature(){}
	
	bool Create(revDevice* device);

	void Set(revGraphicsCommandList& commandList, bool isGraphics = true);
	ID3D12RootSignature* Get() const { return rootSignature; }

private:
	revDevice*              device;        
	ID3D12RootSignature*    rootSignature;     

};

#endif
