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

    const VkRenderPass& GetRenderPass() const { return renderPass; }
    const VkSemaphore& GetSemaphore() const { return semaphore; }
    const VkFence& GetFence() const { return fence; }
private:
    VkRenderPass renderPass;
    VkSemaphore  semaphore;
    VkFence fence;
};

#endif
#endif
