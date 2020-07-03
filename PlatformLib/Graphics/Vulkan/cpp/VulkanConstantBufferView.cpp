#ifdef _USE_VULKAN

#include "VulkanConstantBufferView.h"

void VulkanConstantBufferView::Create(revDevice* device, const VulkanConstantBuffer& cbuffer, VulkanDescriptorSet::Chunk& chunk)
{
    this->device = device;
    descriptorBufferInfo = {};
    descriptorBufferInfo.buffer = *cbuffer.GetHandlePtr();
    descriptorBufferInfo.offset = 0;
    descriptorBufferInfo.range = VK_WHOLE_SIZE;

	chunk.UpdateResource(0, DESCRIPTOR_TYPE::CONSTANT_BUFFER_VIEW, nullptr, &descriptorBufferInfo, nullptr);
}

void VulkanConstantBufferView::Destroy()
{

}

#endif
