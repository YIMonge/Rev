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

    const GRAPHICS_FORMAT GetFormat() const { return format; }
    const revSwapChain&  GetSwapChain() const { return swapChain; }
    const VkExtent2D& GetDisplaySize() const { return displaySize; }
    uint32 GetLength() const { return length; }
private:
    revSwapChain swapChain;
    VkExtent2D displaySize;
    GRAPHICS_FORMAT format;
    uint32 length;
};


#endif
#endif
