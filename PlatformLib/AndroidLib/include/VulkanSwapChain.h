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

    bool Create(VulkanDevice* _device);
    bool CreateFrameBuffer(const VulkanPipelineState& pipelineState);
    void Destroy();

    const GRAPHICS_FORMAT GetFormat() const { return format; }
    const revSwapChain&  GetSwapChain() const { return swapChain; }
    const revRect& GetDisplaySize() const { return displaySize; }

    const VkFramebuffer GetCurrentFrameBuffer() const { return frameBuffers.GetFrameBuffer(frameIndex); }
    VkSemaphore* GetSemaphore(){ return &semaphore; }
    const VkFence& GetFence() const { return fence; }

    uint32 GetLength() const { return length; }

    void PrepareRendering(VulkanCommandList& commandList);
    void EndRendering(VulkanCommandList& commandList);
    bool Present() const;
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
