#ifndef __VKSWAPCHAIN_H__
#define __VKSWAPCHAIN_H__

#ifdef _USE_VULKAN

#include "VkDeviceContext.h"
#include "revArray.h"

class VkSwapChain
{
public:
    VkSwapChain();
    ~VkSwapChain();

    bool Create(const VkDeviceContext& deviceContext);
    void Destroy(const VkDeviceContext& deviceContext);

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
