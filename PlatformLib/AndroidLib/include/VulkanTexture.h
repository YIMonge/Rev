#ifndef __VULKANTEXTURE_H__
#define __VULKANTEXTURE_H__

#ifdef _USE_VULKAN

#include "VulkanDeviceContext.h"

class VulkanTexture
{
public:
    VulkanTexture();
    ~VulkanTexture();

    bool LoadFromFile(const VulkanDeviceContext& deviceContext, const char* path);

private:
    VkSampler sampler;
    VkImage  image;
    VkImageLayout imageLayout;
    VkDeviceMemory deviceMemory;
    VkImageView  imageView;
    uint32 width;
    uint32 height;
};

#endif
#endif

