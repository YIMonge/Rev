#ifndef __DX12ROOTSIGNATURE_H__
#define __DX12ROOTSIGNATURE_H__

#include "DX12Device.h"


class DescriptorSetLayoutDesc
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

struct RootDescriptorDesc
{
	DESCRIPTOR_TYPE type;
	uint32 registerIndex;
	uint32 spaceIndex;
};


class RootConstantsDesc
{
	uint32 registerIndex;
	uint32 spaceIndex;
	uint32 count;
};


class DX12DescriptorSetLayout
{
public:
	DX12DescriptorSetLayout() {}
	virtual ~DX12DescriptorSetLayout() {}

	//bool Create(revDevice* device);

};

class RootSignatureDesc
{

private:
	revArray<DescriptorSetLayoutDesc> descriptorSets;
	revArray<RootDescriptorDesc> rootDescriptors;
	revArray<RootConstantsDesc> rootConstants;
};

class DX12RootSignature
{
public:
	DX12RootSignature() {}
	virtual ~DX12RootSignature(){}
	
	bool Create(revDevice* device);

	void Apply(revGraphicsCommandList& commandList, bool isGraphics = true);
	ID3D12RootSignature* Get() const { return rootSignature; }

private:
	revDevice*              device;        
	ID3D12RootSignature*    rootSignature;     

};

#endif
