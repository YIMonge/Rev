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
		DESCRIPTOR_USAGE GetUsage() const { return usage;  }
	private:
		DESCRIPTOR_TYPE type;
		revArray<Range> ranges;
		SHADER_VISIBILITY visibility;
		DESCRIPTOR_USAGE usage;

	};

	uint32 GetDescriptorSetLayoutCount() const { return static_cast<uint32>(descriptorSets.size()); }
	const DescriptorSetLayoutDesc& GetDescriptorSetLayout(uint32 index) const { return descriptorSets[index]; }

	bool isLocal() const { return local; }
	void AddMaterial(const revMaterial& material);

private:
	void AddShader(const revShader* const shader);
	revArray<DescriptorSetLayoutDesc> descriptorSets;

	bool local;
};


#endif
