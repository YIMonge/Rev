#ifndef __VULKANFRAMEBUFFER_H__
#define __VULKANFRAMEBUFFER_H__
#ifdef _USE_VULKAN

#include "VulkanRenderingBuffer.h"
#include "VulkanDevice.h"
#include "revRect.h"

class VulkanFrameBuffer : public VulkanRenderingBuffer
{
public:
    VulkanFrameBuffer();
    virtual ~VulkanFrameBuffer();

    bool Create(VulkanDevice* device, const VkSwapchainKHR& swapchainKhr, GRAPHICS_FORMAT format, const revRect& rect, uint32 num, const VkRenderPass& renderPass, const VkImageView& depthBufferView = NULL_HANDLE);
    void Destroy();

    const VkFramebuffer& GetFrameBuffer(uint32 index) const { return  framebuffers[index]; }
    uint32 GetFrameBufferCount() const { return static_cast<uint32>(framebuffers.size());}

	const VkImage& GetImage(uint32 index) const { return images[index]; }
private:
	revArray<VkImage>  images;
	revArray<VkImageView> views;
    revArray<VkFramebuffer>  framebuffers;
};

#endif
#endif
