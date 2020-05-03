#include <VulkanSwapChain.h>
#include "VulkanFrameBuffer.h"
#include "Log.h"
#ifdef _USE_VULKAN

VulkanFrameBuffer::VulkanFrameBuffer() :
foramt(VK_FORMAT_A8B8G8R8_UINT_PACK32)
{
}

VulkanFrameBuffer::~VulkanFrameBuffer()
{
}

bool VulkanFrameBuffer::Create(revDevice* device, const VulkanSwapChain &swapChain, const VulkanPipelineState& pipelineState)
{
    this->device = device;
    foramt = ConvertToVKFormat(swapChain.GetFormat());
    uint32 imageCount = 0;
    VkSwapchainKHR  swapchain = swapChain.GetSwapChain();
    VkResult result = vkGetSwapchainImagesKHR(device->GetDevice(), swapchain, &imageCount, nullptr);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    views.resize(imageCount);
    images.resize(imageCount);
    result = vkGetSwapchainImagesKHR(device->GetDevice(), swapchain, &imageCount, &images[0]);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    for(uint32 i = 0; i < imageCount; ++i){
        VkImageViewCreateInfo viewCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .pNext = nullptr,
                .image = images[i],
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .format = foramt,
                .components =
                        {
                            .r = VK_COMPONENT_SWIZZLE_R,
                            .g = VK_COMPONENT_SWIZZLE_G,
                            .b = VK_COMPONENT_SWIZZLE_B,
                            .a = VK_COMPONENT_SWIZZLE_A,
                        },
                .subresourceRange =
                        {
                            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                            .baseMipLevel = 0,
                            .levelCount = 1,
                            .baseArrayLayer = 0,
                            .layerCount = 1
                        },
                .flags = 0,
        };
        result = vkCreateImageView(device->GetDevice(), &viewCreateInfo, nullptr, &views[i]);
        if(result != VK_SUCCESS){
            NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
            return false;
        }
    }

    // create framebuffer
    uint32 swapChainLength = swapChain.GetLength();
    framebuffers.resize(swapChainLength);
    for(uint32 i = 0; i < swapChainLength; ++i){
        VkImageView  attachments[2] = {
                views[i],
                VK_NULL_HANDLE, // TODO: depth
        };
        VkExtent2D size = swapChain.GetDisplaySize();
        VkFramebufferCreateInfo framebufferCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext = nullptr,
            .renderPass = pipelineState.GetRenderPass(),
            .layers = 1,
            .attachmentCount = 1, // if using depth set to 2
            .pAttachments = attachments,
            .width = size.width,
            .height = size.height,
        };

        VkResult result = vkCreateFramebuffer(device->GetDevice(), &framebufferCreateInfo, nullptr, &framebuffers[i]);
        if(result != VK_SUCCESS) {
            NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
            return false;
        }
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
