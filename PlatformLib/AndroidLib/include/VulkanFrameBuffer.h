#ifndef __VULKANFRAMEBUFFER_H__
#define __VULKANFRAMEBUFFER_H__

#ifdef _USE_VULKAN

#include "VulkanDevice.h"
#include "revRect.h"

class VulkanFrameBuffer
{
public:
    VulkanFrameBuffer();
    ~VulkanFrameBuffer();

    bool Create(revDevice* device, const VkSwapchainKHR& swapchainKhr, GRAPHICS_FORMAT format, const revRect& rect, uint32 num, const VkRenderPass& renderPass);
    void Destroy();

    const VkImage& GetFrameBufferImage(uint32 index) const { return images[index]; }
    const VkFramebuffer& GetFrameBuffer(uint32 index) const { return  framebuffers[index]; }
    uint32 GetFrameBufferCount() const { return static_cast<uint32>(framebuffers.size());}

private:
    revDevice* device;
    VkFormat foramt;
    revArray<VkImage>  images;
    revArray<VkImageView> views;
    revArray<VkFramebuffer>  framebuffers;
};

#endif
#endif
