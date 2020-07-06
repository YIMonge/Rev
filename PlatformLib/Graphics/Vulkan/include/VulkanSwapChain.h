#ifndef __VULKANSWAPCHAIN_H__
#define __VULKANSWAPCHAIN_H__
#ifdef _USE_VULKAN

#include "VulkanDevice.h"
#include "revArray.h"
#include "VulkanFrameBuffer.h"
#include "VulkanDepthBuffer.h"
#include "VulkanPipelineState.h"

class VulkanSwapChain
{
public:
    VulkanSwapChain();
    ~VulkanSwapChain();

    bool Create(VulkanDevice* device);
    bool CreateFrameBuffer(const VulkanRenderPass& renderPass);
	bool CreateDepthBuffer();
    void Destroy();

    const GRAPHICS_FORMAT GetFormat() const { return format; }
    const revSwapChain&  GetSwapChain() const { return swapChain; }
    const revRect& GetDisplaySize() const { return displaySize; }

    uint32 GetCurrentFrameIndex() const { return frameIndex; }

    const VulkanFrameBuffer& GetFrameBuffer() const { return frameBuffers; }
	const VulkanDepthBuffer& GetDepthBuffer() const { return depthBuffer; }

	VkSemaphore* GetSemaphore(){ return &semaphore; }
    uint32 GetLength() const { return length; }

    bool Present();
private:
    VulkanDevice* device;
    
    revSwapChain swapChain;
    revRect displaySize;
    GRAPHICS_FORMAT format;
    uint32 length;
    uint32 frameIndex;

	VkSemaphore  semaphore;
    VulkanFrameBuffer frameBuffers;
	VulkanDepthBuffer depthBuffer;
};


#endif
#endif
