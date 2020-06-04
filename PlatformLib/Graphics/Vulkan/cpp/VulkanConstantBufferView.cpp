#ifdef _USE_VULKAN

#include "VulkanConstantBufferView.h"

void VulkanConstantBufferView::Create(revDevice* device, const VulkanConstantBuffer& cbuffer, VulkanDescriptorSet* descriptorSet)
{
    this->device = device;


}

void VulkanConstantBufferView::Destroy()
{

}

#endif
