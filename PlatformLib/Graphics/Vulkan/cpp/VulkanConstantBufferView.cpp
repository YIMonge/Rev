#ifdef _USE_VULKAN

#include "VulkanConstantBufferView.h"

void VulkanConstantBufferView::Create(revDevice* device, const VulkanConstantBuffer& cbuffer, VulkanDescriptorSet::Chunk& chunk)
{
    this->device = device;
    descriptorBufferInfo = {};
    descriptorBufferInfo.buffer = *cbuffer.GetHandlePtr();
    descriptorBufferInfo.offset = 0;
    descriptorBufferInfo.range = VK_WHOLE_SIZE;

    VkWriteDescriptorSet* writeDescriptorSet = chunk.GetVkWriteDescriptorSet();
    for(uint32 i = 0; i < chunk.GetNumOfDescriptors(); ++i){
        writeDescriptorSet[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeDescriptorSet[i].pBufferInfo = &descriptorBufferInfo;
    }

}

void VulkanConstantBufferView::Destroy()
{

}

#endif
