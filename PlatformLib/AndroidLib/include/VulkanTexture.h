#ifndef __VULKANTEXTURE_H__
#define __VULKANTEXTURE_H__

#ifdef _USE_VULKAN

#include "VulkanDeviceContext.h"
#include "ITexture.h"

class VulkanTexture : public ITexture
{
public:
    VulkanTexture();
    virtual ~VulkanTexture();

    bool LoadFromFile(const VulkanDeviceContext& deviceContext, const char* path);
    VkDescriptorImageInfo GetDescriptorImageInfo();

private:
    bool AllocateMemoryTypeFromProperties(const VulkanDeviceContext& deviceContext, uint32 typeBits, VkFlags requimentMask, uint32* typeIndex);

    VkImage  image;
    VkDeviceMemory deviceMemory;
    uint32 width;
    uint32 height;

    VkImageLayout imageLayout;

};

#endif
#endif

