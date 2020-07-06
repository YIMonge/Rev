#ifndef __VULKANRENDERINGBUFFER_H__
#define __VULKANRENDERINGBUFFER_H__
#ifdef _USE_VULKAN

#include "VulkanDevice.h"
#include "revRect.h"

class VulkanRenderingBuffer
{
public:
    VulkanRenderingBuffer();
    virtual ~VulkanRenderingBuffer();

    virtual void Destroy();

	VkFormat GetVulkanFormat() const { return format; }
    
protected:
    bool CreateImage(const revRect& rect, GRAPHICS_FORMAT format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    bool CreateImageView(const VkImage& image, GRAPHICS_FORMAT format, VkImageAspectFlags aspectFlags, VkImageView& imageView);

    revDevice* device;
    VkFormat format;
};

#endif
#endif
