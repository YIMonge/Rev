#include <revColor.h>
#include "VulkanRenderer.h"
#include "Log.h" // AndroidLib doesn't depend on rev
#ifdef _USE_VULKAN

VulkanRenderer::VulkanRenderer()
{
    clearValue = {
            .color.float32[0] = revColor::white.r,
            .color.float32[1] = revColor::white.g,
            .color.float32[2] = revColor::white.b,
            .color.float32[3] = revColor::white.a,
    };
}


VulkanRenderer::~VulkanRenderer()
{
}


void VulkanRenderer::StartUp(Window* window, const GraphicsDesc& desc)
{
    if(!context.Create(*window)) return;
    if(!swapChain.Create(context)) return;
    if(!renderInfo.Create(context, swapChain)) return;
    if(!frameBuffer.Create(context, swapChain, renderInfo)) return;
    if(!CreateCommandPool()) return;
}

void VulkanRenderer::ShutDown()
{
    frameBuffer.Destroy(context);
    renderInfo.Destroy(context);
    swapChain.Destroy(context);
    context.Destroy();
}

void VulkanRenderer::RenderBegin()
{
    uint32 index;
    VkDevice device = context.GetDevice();
    VkResult result;
    result = vkAcquireNextImageKHR(device,
            swapChain.GetSwapChain(),
            UINT64_MAX,
            renderInfo.GetSemaphore(),
            VK_NULL_HANDLE,
            &index);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
    }

    result = vkResetFences(device, 1, &renderInfo.GetFence());
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
    }

    VkPipelineStageFlags  waitStageMask =  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submitInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &renderInfo.GetSemaphore(),
            .pWaitDstStageMask = &waitStageMask,
            .commandBufferCount = 1,
            .pCommandBuffers = &commandBuffers[index],
            .signalSemaphoreCount = 0,
            .pSignalSemaphores = nullptr,
    };
    result = vkQueueSubmit(context.GetQueue(), 1, &submitInfo, renderInfo.GetFence());
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
    }

    result = vkWaitForFences(device, 1, &renderInfo.GetFence(), VK_TRUE, 100000000);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
    }
    VkPresentInfoKHR presentInfo = {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext = nullptr,
            .swapchainCount = 1,
            .pSwapchains = &swapChain.GetSwapChain(),
            .pImageIndices = &index,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores = nullptr,
            .pResults = &result,
    };
    vkQueuePresentKHR(context.GetQueue(), &presentInfo);
}

void VulkanRenderer::RenderEnd()
{

}

void VulkanRenderer::Clear(bool clear_color, bool clear_depth, const revColor& fill_color)
{
    clearValue = {
        .color.float32[0] = fill_color.r,
        .color.float32[1] = fill_color.g,
        .color.float32[2] = fill_color.b,
        .color.float32[3] = fill_color.a,
    };
}

void VulkanRenderer::SwapBuffers()
{

}

void VulkanRenderer::SetBlendFunc(BLEND_FUNC func)
{

}

void VulkanRenderer::SetAlphaTest(ALPHA_TEST func, float value)
{

}

bool VulkanRenderer::CreateCommandPool()
{
    VkCommandPoolCreateInfo commandPoolCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = 0,
    };

    const VkDevice& device = context.GetDevice();
    VkResult result = vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, &commandPool);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    commandBuffers.Resize(swapChain.GetLength());
    for(int i = 0; i < commandBuffers.Count(); ++i){
        VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                .pNext = nullptr,
                .commandPool = commandPool,
                .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                .commandBufferCount = 1,
        };
        result = vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, &commandBuffers[i]);
        if(result != VK_SUCCESS) {
            NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
            return false;
        }

        VkCommandBufferBeginInfo commandBufferBeginInfo = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                .pNext = nullptr,
                .flags = 0,
                .pInheritanceInfo = nullptr,
        };
        result = vkBeginCommandBuffer(commandBuffers[i], &commandBufferBeginInfo);
        if(result != VK_SUCCESS) {
            NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
            return false;
        }

        // Image Layout
        setImageLayout(commandBuffers[i], frameBuffer.GetImages()[i],
                VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

        VkRenderPassBeginInfo renderPassBeginInfo = {
                .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                .pNext = nullptr,
                .renderPass = renderInfo.GetRenderPass(),
                .framebuffer = frameBuffer.GetFrameBuffers()[i],
                .renderArea = {
                        .offset =
                        {
                            .x = 0,
                            .y = 0,
                        },
                        .extent = swapChain.GetDisplaySize(),
                },
                .clearValueCount = 1,
                .pClearValues = &clearValue,
        };
        vkCmdBeginRenderPass(commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdEndRenderPass(commandBuffers[i]);
        setImageLayout(commandBuffers[i], frameBuffer.GetImages()[i],
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);
        result = vkEndCommandBuffer(commandBuffers[i]);
        if(result != VK_SUCCESS) {
            NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
            return false;
        }
    }
    return true;
}

void VulkanRenderer::setImageLayout(VkCommandBuffer commandBuffer, VkImage image,
                                    VkImageLayout oldImageLayout, VkImageLayout newImageLayout,
                                    VkPipelineStageFlags srcStages,
                                    VkPipelineStageFlags destStages)
{
    VkImageMemoryBarrier imageMemoryBarrier = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext = nullptr,
            .srcAccessMask = 0,
            .dstAccessMask = 0,
            .oldLayout = oldImageLayout,
            .newLayout = newImageLayout,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = image,
            .subresourceRange = {
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel = 0,
                    .levelCount = 1,

            }
    };
    switch (oldImageLayout) {
        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_PREINITIALIZED:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
            break;

        default:
            break;
    }

    switch (newImageLayout) {
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            break;

        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            break;

        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            imageMemoryBarrier.dstAccessMask =
                    VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            break;

        default:
            break;
    }

    vkCmdPipelineBarrier(commandBuffer, srcStages, destStages, 0, 0, nullptr, 0, nullptr, 1,
                         &imageMemoryBarrier);
}

void VulkanRenderer::DestroyCommandPool()
{

}


#endif
