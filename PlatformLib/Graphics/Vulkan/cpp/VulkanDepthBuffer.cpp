#ifdef _USE_VULKAN

#include "VulkanDepthBuffer.h"
#include "Log.h"

VulkanDepthBuffer::VulkanDepthBuffer() :
	image(NULL_HANDLE),
	view(NULL_HANDLE)
{
}

VulkanDepthBuffer::~VulkanDepthBuffer()
{
}

bool VulkanDepthBuffer::Create(VulkanDevice* device, const VkSwapchainKHR& swapchainKhr, GRAPHICS_FORMAT format, const revRect& rect)
{
	this->device = device;
	this->format = ConvertToVKFormat(format);
	if (!CreateImage(rect, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image, imageMemory)) {
		NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
		return false;
	}

	if (!CreateImageView(image, format, VK_IMAGE_ASPECT_DEPTH_BIT, view)){
		NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
		return false;
	}

	return true;
}

void VulkanDepthBuffer::Destroy()
{
	vkDestroyImageView(device->GetDevice(), view, nullptr);
	vkDestroyImage(device->GetDevice(), image, nullptr);
}



#endif