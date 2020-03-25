#ifndef __VULKANTEXTURE_H__
#define __VULKANTEXTURE_H__

#ifdef _USE_VULKAN

#include "VulkanDeviceContext.h"
#include "revTexture.h"

class VulkanTexture : public revTexture
{
public:
    VulkanTexture();
    virtual ~VulkanTexture();

    VkDescriptorImageInfo GetDescriptorImageInfo();

private:
    bool CreateTexture(const revDeviceContext& deviceContext, uint8* imageData);
    bool AllocateMemoryTypeFromProperties(const VulkanDeviceContext& deviceContext, uint32 typeBits, VkFlags requimentMask, uint32* typeIndex);

    VkImage  image;
    VkDeviceMemory deviceMemory;
    VkImageLayout imageLayout;
};

#endif
#endif

