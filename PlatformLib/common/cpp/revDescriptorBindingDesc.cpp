#include "revDescriptorBindingDesc.h"

void revDescriptorBindingDesc::AddMaterial(const revMaterial& material)
{
	const revShader* vertexShader = material.GetShader(SHADER_TYPE::VERTX);
	const revShader* fragmentShader = material.GetShader(SHADER_TYPE::FRAGMENT);
	AddShader(vertexShader);
	AddShader(fragmentShader);
	/*
	revArray<DescriptorSetLayoutDesc> descriptorSets;
	revArray<RootConstantDesc> rootConstants;
	*/
}

void revDescriptorBindingDesc::AddShader(const revShader* const shader)
{
	auto& textureBindings = shader->GetTextureBindings();

	// TODO: constants buffer 
	if (!textureBindings.empty()) {
		DescriptorSetLayoutDesc setDesc;
		setDesc.visibility = SHADER_VISIBILITY::PIXEL;
		setDesc.type = DESCRIPTOR_TYPE::TEXTURE_SHADER_RESOURCE_VIEW;
		for (uint32 i = 0; i < textureBindings.size(); ++i) {
			DescriptorSetLayoutDesc::Range range;
			//range.type = DESCRIPTOR_TYPE::TEXTURE_SHADER_RESOURCE_VIEW;
			range.registerIndex = textureBindings[i].GetRegisterIndex();
			range.count = textureBindings[i].GetCount();
			range.space = textureBindings[i].GetSpace();
			setDesc.ranges.push_back(range);
		}
		descriptorSets.push_back(setDesc);
	}

	auto& samplerBindings = shader->GetSamplerBindings();
	if (!samplerBindings.empty()) {
		DescriptorSetLayoutDesc setDesc;
		setDesc.visibility = SHADER_VISIBILITY::PIXEL;
		setDesc.type = DESCRIPTOR_TYPE::SAMPLER;
		for (uint32 i = 0; i < samplerBindings.size(); ++i) {
			DescriptorSetLayoutDesc::Range range;
			//range.type = DESCRIPTOR_TYPE::SAMPLER;
			range.registerIndex = samplerBindings[i].GetRegisterIndex();
			range.count = samplerBindings[i].GetCount();
			range.space = samplerBindings[i].GetSpace();
			setDesc.ranges.push_back(range);
		}
		descriptorSets.push_back(setDesc);
	}
}
