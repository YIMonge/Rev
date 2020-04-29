#ifndef __VULKANSWAPCHAIN_H__
#define __VULKANSWAPCHAIN_H__

#ifdef _USE_VULKAN

#include "VulkanDevice.h"
#include "revArray.h"

class VulkanSwapChain
{
public:
    VulkanSwapChain();
    ~VulkanSwapChain();

    bool Create(VulkanDevice* _device);
    void Destroy();

    const GRAPHICS_FORMAT GetFormat() const { return format; }
    const revSwapChain&  GetSwapChain() const { return swapChain; }
    const VkExtent2D& GetDisplaySize() const { return displaySize; }

    const VkSemaphore& GetSemaphore() const { return semaphore; }
    const VkFence& GetFence() const { return fence; }

    uint32 GetLength() const { return length; }
    bool Present() const;
    bool WaitForPreviousFrame();
private:
    VulkanDevice* device;
    revSwapChain swapChain;
    VkExtent2D displaySize;
    GRAPHICS_FORMAT format;
    uint32 length;
    uint32 frameIndex;

    VkSemaphore  semaphore;
    VkFence fence;
};


#endif
#endif
