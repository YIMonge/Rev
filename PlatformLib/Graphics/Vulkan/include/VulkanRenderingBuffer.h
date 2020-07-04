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

    virtual bool Create(VulkanDevice* device, const VkSwapchainKHR& swapchainKhr, GRAPHICS_FORMAT format, const revRect& rect, uint32 num, const VkRenderPass& renderPass) = 0;
    virtual void Destroy();

    const VkImage& GetImage(uint32 index) const { return images[index]; }
protected:
    bool CreateImage(const revRect& rect, GRAPHICS_FORMAT format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    bool CreateImageView(const VkImage& image, GRAPHICS_FORMAT format, VkImageAspectFlags aspectFlags, VkImageView& imageView);

    revDevice* device;
    VkFormat foramt;
    revArray<VkImage>  images;
    revArray<VkImageView> views;
};

#endif
#endif
