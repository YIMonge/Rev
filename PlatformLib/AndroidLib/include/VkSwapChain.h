#ifndef __VKSWAPCHAIN_H__
#define __VKSWAPCHAIN_H__

#ifdef _USE_VULKAN

#include "VkDeviceContext.h"

class VkSwapChain
{
public:
    VkSwapChain();
    ~VkSwapChain();

    bool Create(VkDeviceContext& deviceContext);
    void Destroy(VkDeviceContext& deviceContext);
private:
    VkSwapchainKHR swapChain;
    VkExtent2D displaySize;
    VkFormat format;
    uint32 length;
};


#endif
#endif
