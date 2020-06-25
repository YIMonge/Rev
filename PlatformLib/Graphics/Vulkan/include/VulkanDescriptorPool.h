#ifndef __VULKANDESCRIPTORPOOL_H__
#define __VULKANDESCRIPTORPOOL_H__
#ifdef _USE_VULKAN
#include "VulkanDevice.h"

class VulkanDescriptorPool
{
public:
    VulkanDescriptorPool() :
    device(nullptr)
    {}

    virtual ~VulkanDescriptorPool(){}

    bool Create(revDevice* device, uint32 cbufferResourceMaxNum, uint32 textureResourceMaxNum);

    VkDescriptorPool GetHandle() const { return descriptorPool; }

private:
    VulkanDevice* device;
    VkDescriptorPool descriptorPool;
};

#endif
#endif