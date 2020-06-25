#ifndef __VULKANSWAPCHAIN_H__
#define __VULKANSWAPCHAIN_H__
#ifdef _USE_VULKAN

#include "VulkanDevice.h"
#include "revArray.h"
#include "VulkanFrameBuffer.h"
#include "VulkanPipelineState.h"

class VulkanSwapChain
{
public:
    VulkanSwapChain();
    ~VulkanSwapChain();

    bool Create(VulkanDevice* device);
    bool CreateFrameBuffer(const VulkanRenderPass& renderPass);
    void Destroy();

    const GRAPHICS_FORMAT GetFormat() const { return format; }
    const revSwapChain&  GetSwapChain() const { return swapChain; }
    const revRect& GetDisplaySize() const { return displaySize; }

    uint32 GetCurrentFrameIndex() const { return frameIndex; }
    const VulkanFrameBuffer& GetFrameBuffer() const { return frameBuffers; }
    VkSemaphore* GetSemaphore(){ return &semaphore; }
    const VkFence& GetFence() const { return fence; }

    uint32 GetLength() const { return length; }

    bool Present();
    bool WaitForPreviousFrame();
private:
    VulkanDevice* device;
    
    revSwapChain swapChain;
    revRect displaySize;
    GRAPHICS_FORMAT format;
    uint32 length;
    uint32 frameIndex;

    VulkanFrameBuffer frameBuffers;
    VkSemaphore  semaphore;
    VkFence fence;
};


#endif
#endif
