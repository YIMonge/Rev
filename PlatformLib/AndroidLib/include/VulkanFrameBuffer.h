#ifndef __VULKANFRAMEBUFFER_H__
#define __VULKANFRAMEBUFFER_H__

#ifdef _USE_VULKAN

#include "VulkanSwapChain.h"
#include "VulkanPipelineState.h"

class VulkanFrameBuffer
{
public:
    VulkanFrameBuffer();
    ~VulkanFrameBuffer();

    bool Create(revDevice* device, const VulkanSwapChain& swapChain, const VulkanPipelineState& pipelineState);
    void Destroy();

    const revArray<VkImage>& GetImages() const { return images; }
    const revArray<VkImageView>& GetViews() const { return views; }
    const revArray<VkFramebuffer>& GetFrameBuffers() const { return  framebuffers; }
private:
    revDevice* device;
    VkFormat foramt;
    revArray<VkImage>  images;
    revArray<VkImageView> views;
    revArray<VkFramebuffer>  framebuffers;
};

#endif
#endif
