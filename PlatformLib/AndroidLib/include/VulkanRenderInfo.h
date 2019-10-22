#ifndef __VULKANRENDERINFO_H__
#define __VULKANRENDERINFO_H__

#ifdef _USE_VULKAN

#include "VulkanDeviceContext.h"
#include "VulkanSwapChain.h"

class VulkanRenderInfo
{
public:
    bool Create(const VulkanDeviceContext& deviceContext, const VulkanSwapChain& swapChain);
    void Destroy(const VulkanDeviceContext& deviceContext);

private:
    VkRenderPass renderPass;
    VkCommandPool  cmdPool;
    VkCommandBuffer* cmdBuffer;
    uint32 cmdBufferLength;
    //VkSemaphore  semaphore;
    //VkFence fence;
};

#endif
#endif
