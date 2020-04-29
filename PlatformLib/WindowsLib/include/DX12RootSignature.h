#ifndef __DX12ROOTSIGNATURE_H__
#define __DX12ROOTSIGNATURE_H__

#include "DX12Device.h"
#include "revMaterial.h"

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

	uint32 GetRangeCount() const { return static_cast<uint32>(ranges.size()); }
	const Range& GetRange(uint32 index) const { return ranges[index]; }
	SHADER_VISIBILITY GetShaderVisiblity() const { return visibility; }

#ifdef _DEBUG
	void AddRange(const Range& range) { ranges.push_back(range); }
	void SetShaderVisiblity(SHADER_VISIBILITY v) { visibility = v; }
#endif

private:
	revArray<Range> ranges;
	SHADER_VISIBILITY visibility;
};

struct RootDescriptorDesc
{
	DESCRIPTOR_TYPE type;
	uint32 registerIndex;
	uint32 spaceIndex;
	SHADER_VISIBILITY visibility;
};


struct RootConstantDesc
{
	uint32 registerIndex;
	uint32 spaceIndex;
	uint32 count;
	SHADER_VISIBILITY visibility;
};



class RootSignatureDesc
{
public:

	void AddDescriptorSetLayout(const DescriptorSetLayoutDesc& desc) { descriptorSets.push_back(desc); }
	void AddRootDescriptor(const RootDescriptorDesc& desc) { rootDescriptors.push_back(desc); }
	void AddRootConstant(const RootConstantDesc& desc) { rootConstants.push_back(desc); }

	uint32 GetDescriptorSetLayoutCount() const { return static_cast<uint32>(descriptorSets.size()); }
	const DescriptorSetLayoutDesc& GetDescriptorSetLayout(uint32 index) const { return descriptorSets[index]; }
	uint32 GetRootDescriptorDescCount() const { return static_cast<uint32>(rootDescriptors.size()); }
	const RootDescriptorDesc& GetRootDescriptorDesc(uint32 index) const { return rootDescriptors[index]; }
	uint32 GetRootConstantDescCount() const { return static_cast<uint32>(rootConstants.size()); }
	const RootConstantDesc& GetRootConstantDesc(uint32 index) const { return rootConstants[index]; }

	bool isLocal() const { return local; }

	void AddMaterial(const revMaterial& material);

private:
	void AddShader(const revShader* const shader);

	revArray<DescriptorSetLayoutDesc> descriptorSets;
	revArray<RootDescriptorDesc> rootDescriptors;
	revArray<RootConstantDesc> rootConstants;
	bool local;
};

class DX12RootSignature
{
public:
	DX12RootSignature() :
		sizeInBytes(0)

	{}
	virtual ~DX12RootSignature(){}
	
	bool Create(revDevice* device);
	bool Create(revDevice* device, const RootSignatureDesc& desc);

	void Apply(revGraphicsCommandList& commandList, bool isGraphics = true);
	ID3D12RootSignature* Get() const { return rootSignature; }

private:
	revDevice*              device;        
	ID3D12RootSignature*    rootSignature;     
	uint32 sizeInBytes;
	revArray<uint32> byteOffsets;
};

#endif
