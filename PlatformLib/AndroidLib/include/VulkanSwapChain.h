#ifndef __VULKANSWAPCHAIN_H__
#define __VULKANSWAPCHAIN_H__

#ifdef _USE_VULKAN

#include "VulkanDeviceContext.h"
#include "revArray.h"

class VulkanSwapChain
{
public:
    VulkanSwapChain();
    ~VulkanSwapChain();

    bool Create(const VulkanDeviceContext& deviceContext);
    void Destroy(const VulkanDeviceContext& deviceContext);

    VkFormat GetFormat() const { return format; }
private:
    VkSwapchainKHR swapChain;
    VkExtent2D displaySize;
    VkFormat format;
    uint32 length;

    // FrameBuffers
    revArray<VkImage> images;
    revArray<VkImageView> views;
    revArray<VkFramebuffer> frameBuffers;
};


#endif
#endif
