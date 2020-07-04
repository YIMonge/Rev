#ifdef _USE_VULKAN

#include <VulkanSwapChain.h>
#include "VulkanRenderingBuffer.h"
#include "Log.h"

VulkanRenderingBuffer::VulkanRenderingBuffer() :
    foramt(VK_FORMAT_A8B8G8R8_UINT_PACK32)
{
}

VulkanRenderingBuffer::~VulkanRenderingBuffer()
{
}

bool VulkanRenderingBuffer::CreateImage(const revRect& rect, GRAPHICS_FORMAT format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
{
    VkImageCreateInfo imageCreateInfo = {};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.extent.width = rect.w;
    imageCreateInfo.extent.height = rect.h;
    imageCreateInfo.extent.depth = 1;
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.format = ConvertToVKFormat(format);
    imageCreateInfo.tiling = tiling;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.usage = usage;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    auto device = this->device->GetDevice();
    VkResult result = vkCreateImage(device, &imageCreateInfo, nullptr, &image);
    if (result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
        return false;
    }
    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(device, image, &memoryRequirements);

    VkMemoryAllocateInfo memoryAllocateInfo = {};
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
	if (!MapMemoryTypeToIndex(this->device->GetAdapter(),
		memoryRequirements.memoryTypeBits,
		properties,
		&memoryAllocateInfo.memoryTypeIndex)) {
		NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
		return false;
	}
	result = vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &imageMemory);
	if (result != VK_SUCCESS) {
		NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
		return false;
	}
	vkBindImageMemory(device, image, imageMemory, 0);

    return true;
}

bool VulkanRenderingBuffer::CreateImageView(const VkImage& image, GRAPHICS_FORMAT format, VkImageAspectFlags aspectFlags, VkImageView& imageView)
{
    VkImageViewCreateInfo viewCreateInfo = {};
    viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewCreateInfo.image = image;
    viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewCreateInfo.format = foramt;
    viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewCreateInfo.subresourceRange.baseMipLevel = 0;
    viewCreateInfo.subresourceRange.levelCount = 1;
    viewCreateInfo.subresourceRange.baseArrayLayer = 0;
    viewCreateInfo.subresourceRange.layerCount = 1;
    viewCreateInfo.flags = 0;

    VkResult result = vkCreateImageView(device->GetDevice(), &viewCreateInfo, nullptr, &imageView);
    if (result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
        return false;
    }
    return true;
}


void VulkanRenderingBuffer::Destroy()
{
    VkDevice vkDevice = device->GetDevice();
    for (uint32 i = 0; i < static_cast<uint32>(views.size()); ++i) {
        vkDestroyImageView(vkDevice, views[i], nullptr);
    }
}
#endif
