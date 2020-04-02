#ifndef __VULKANTEXTURE_H__
#define __VULKANTEXTURE_H__

#ifdef _USE_VULKAN

#include "VulkanDevice.h"
#include "revTexture.h"

class VulkanTexture : public revTexture
{
public:
    VulkanTexture();
    virtual ~VulkanTexture();

    VkDescriptorImageInfo GetDescriptorImageInfo();
private:
    bool CreateTexture(const revDevice& device, uint8* imageData);
    bool AllocateMemoryTypeFromProperties(const VulkanDevice& device, uint32 typeBits, VkFlags requimentMask, uint32* typeIndex);

    VkImage  image;
    VkDeviceMemory deviceMemory;
};

#endif
#endif

