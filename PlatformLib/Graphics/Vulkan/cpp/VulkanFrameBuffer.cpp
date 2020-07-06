#ifdef _USE_VULKAN

#include <VulkanSwapChain.h>
#include "VulkanFrameBuffer.h"
#include "Log.h"

VulkanFrameBuffer::VulkanFrameBuffer()
{
}

VulkanFrameBuffer::~VulkanFrameBuffer()
{
}

bool VulkanFrameBuffer::Create(VulkanDevice* device, const VkSwapchainKHR& swapchainKhr, GRAPHICS_FORMAT format, const revRect& rect, uint32 num, const VkRenderPass& renderPass, const VkImageView& depthBufferView)
{
    this->device = device;
    this->format = ConvertToVKFormat(format);
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


    for (uint32 i = 0; i < imageCount; ++i) {
        if (!CreateImageView(images[i], format, VK_IMAGE_ASPECT_COLOR_BIT, views[i])) {
            NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
            return false;
        }
    }

    // create framebuffer
    framebuffers.resize(num);
    for(uint32 i = 0; i < num; ++i){
        VkImageView  attachments[2] = {
                views[i],
                depthBufferView
        };
        VkFramebufferCreateInfo framebufferCreateInfo = {};
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.pNext = nullptr;
        framebufferCreateInfo.renderPass = renderPass;
        framebufferCreateInfo.layers = 1;
		framebufferCreateInfo.attachmentCount = depthBufferView != NULL_HANDLE ? 2 : 1; 
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
        //vkDestroyImage(vkDevice, images[i], nullptr);
    }
}
#endif
