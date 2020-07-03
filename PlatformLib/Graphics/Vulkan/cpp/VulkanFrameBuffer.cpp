#ifdef _USE_VULKAN

#include <VulkanSwapChain.h>
#include "VulkanFrameBuffer.h"
#include "Log.h"

VulkanFrameBuffer::VulkanFrameBuffer() :
foramt(VK_FORMAT_A8B8G8R8_UINT_PACK32)
{
}

VulkanFrameBuffer::~VulkanFrameBuffer()
{
}

bool VulkanFrameBuffer::Create(VulkanDevice* device, const VkSwapchainKHR& swapchainKhr, GRAPHICS_FORMAT format, const revRect& rect, uint32 num, const VkRenderPass& renderPass)
{
    this->device = device;
    this->foramt = ConvertToVKFormat(format);
    uint32 imageCount = 0;
    VkResult result = vkGetSwapchainImagesKHR(device->GetDevice(), swapchainKhr, &imageCount, nullptr);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    views.resize(imageCount);
    images.resize(imageCount);
    result = vkGetSwapchainImagesKHR(device->GetDevice(), swapchainKhr, &imageCount, images.data());
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    for(uint32 i = 0; i < imageCount; ++i){
        VkImageViewCreateInfo viewCreateInfo = {};
        viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewCreateInfo.image = images[i];
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
        
        result = vkCreateImageView(device->GetDevice(), &viewCreateInfo, nullptr, &views[i]);
        if(result != VK_SUCCESS){
            NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
            return false;
        }
    }

    // create framebuffer
    framebuffers.resize(num);
    for(uint32 i = 0; i < num; ++i){
        VkImageView  attachments[2] = {
                views[i],
                VK_NULL_HANDLE, // TODO: depth
        };
        VkFramebufferCreateInfo framebufferCreateInfo = {};
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.pNext = nullptr;
        framebufferCreateInfo.renderPass = renderPass;
        framebufferCreateInfo.layers = 1;
        framebufferCreateInfo.attachmentCount = 1; // if using depth set to 2
        framebufferCreateInfo.pAttachments = attachments;
        framebufferCreateInfo.width = rect.w;
        framebufferCreateInfo.height = rect.h;
       

        VkResult result = vkCreateFramebuffer(device->GetDevice(), &framebufferCreateInfo, nullptr, &framebuffers[i]);
        if(result != VK_SUCCESS) {
            NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
            return false;
        }
    }

	for (uint32 i = 0; i < num; ++i) {
		device->GetGlobalCommandList().AddTransitionBarrier(images[i],
			VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
	}


    return true;
}

void VulkanFrameBuffer::Destroy()
{
    VkDevice vkDevice =  device->GetDevice();
    for(int i = 0; i < framebuffers.size(); ++i){
        vkDestroyFramebuffer(vkDevice, framebuffers[i], nullptr);
        vkDestroyImageView(vkDevice, views[i], nullptr);
        vkDestroyImage(vkDevice, images[i], nullptr);
    }
}
#endif
