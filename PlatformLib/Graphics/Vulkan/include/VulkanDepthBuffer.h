#ifndef __VULKANDEPTHBUFFER_H__
#define __VULKANDEPTHBUFFER_H__
#ifdef _USE_VULKAN

#include "VulkanRenderingBuffer.h"

class VulkanDepthBuffer : public VulkanRenderingBuffer
{
public:
	VulkanDepthBuffer();
	virtual ~VulkanDepthBuffer();

	virtual bool Create(VulkanDevice* device, const VkSwapchainKHR& swapchainKhr, GRAPHICS_FORMAT format, const revRect& rect);
	virtual void Destroy();


private:
	VkDeviceMemory imageMemory;
};

#endif
#endif