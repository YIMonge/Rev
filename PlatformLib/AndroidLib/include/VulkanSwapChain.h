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
    VkSwapchainKHR  GetSwapChain() const { return swapChain; }
    VkExtent2D GetDisplaySize() const { return displaySize; }
    uint32 GetLength() const { return length; }
private:
    VkSwapchainKHR swapChain;
    VkExtent2D displaySize;
    VkFormat format;
    uint32 length;
};


#endif
#endif
