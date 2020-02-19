#ifndef __VULKANFRAMEBUFFER_H__
#define __VULKANFRAMEBUFFER_H__

#ifdef _USE_VULKAN

#include "VulkanRenderInfo.h"

class VulkanFrameBuffer
{
public:
    VulkanFrameBuffer();
    ~VulkanFrameBuffer();

    bool Create(const VulkanDeviceContext& deviceContext, const VulkanSwapChain& swapChain, const VulkanRenderInfo& renderInfo);
    void Destroy(const VulkanDeviceContext& deviceContext);

    const revArray<VkImage>& GetImages() const { return images; }
    const revArray<VkImageView>& GetViews() const { return views; }
    const revArray<VkFramebuffer>& GetFrameBuffers() const { return  framebuffers; }
private:
    VkFormat foramt;
    revArray<VkImage>  images;
    revArray<VkImageView> views;
    revArray<VkFramebuffer>  framebuffers;
};

#endif
#endif
