#ifdef _USE_VULKAN

#include "VulkanSamplerView.h"

void VulkanSamplerView::Create(const VulkanSampler& sampler, VulkanDescriptorSet::Chunk& chunk)
{
	descriptorImageInfo = {};
	descriptorImageInfo.sampler = sampler.GetHandle();
	descriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	chunk.UpdateResource(0, DESCRIPTOR_TYPE::SAMPLER, &descriptorImageInfo, nullptr, nullptr);
}

void VulkanSamplerView::Destroy()
{

}

#endif
