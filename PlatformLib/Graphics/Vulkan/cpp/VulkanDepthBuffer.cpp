#ifdef _USE_VULKAN

#include "VulkanDepthBuffer.h"
#include "Log.h"

VulkanDepthBuffer::VulkanDepthBuffer()
{
}

VulkanDepthBuffer::~VulkanDepthBuffer()
{
}

bool VulkanDepthBuffer::Create(VulkanDevice* device, const VkSwapchainKHR& swapchainKhr, GRAPHICS_FORMAT format, const revRect& rect)
{
	images.resize(1);
	if (!CreateImage(rect, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, images[0], imageMemory)) {
		NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
		return false;
	}

	views.resize(1);
	if (!CreateImageView(images[0], format, VK_IMAGE_ASPECT_DEPTH_BIT, views[0])){
		NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
		return false;
	}

	return true;
}

void VulkanDepthBuffer::Destroy()
{
	for (uint32 i = 0; i < static_cast<uint32>(views.size()); ++i) {
		vkDestroyImageView(device->GetDevice(), views[i], nullptr);
	}
	views.clear();

	for (uint32 i = 0; i < static_cast<uint32>(images.size()); ++i) {
		vkDestroyImage(device->GetDevice(), images[i], nullptr);
	}
	images.clear();
}



#endif