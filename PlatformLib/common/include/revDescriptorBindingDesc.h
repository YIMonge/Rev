#ifndef __REVDESCRIPTORBINDINGDESC_H__
#define __REVDESCRIPTORBINDINGDESC_H__

#include "revMaterial.h"

class revDescriptorBindingDesc
{
public:
	class DescriptorSetLayoutDesc
	{
		friend class revDescriptorBindingDesc;
	public:
		struct Range
		{
			//DESCRIPTOR_TYPE type;
			uint32 registerIndex;
			uint32 count;
			uint32 space;
		};

		uint32 GetRangeCount() const { return static_cast<uint32>(ranges.size()); }
		const Range& GetRange(uint32 index) const { return ranges[index]; }
		SHADER_VISIBILITY GetShaderVisiblity() const { return visibility; }
		DESCRIPTOR_TYPE GetDescriptorType() const { return type; }

	private:
		DESCRIPTOR_TYPE type;
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


#endif
