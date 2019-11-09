#include <revColor.h>
#include "VulkanRenderer.h"
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
    context.Create(*window);
    swapChain.Create(context);
    renderInfo.Create(context, swapChain);
    frameBuffer.Create(context, swapChain, renderInfo);

    CreateCommandPool();

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

    VkDevice device = context.GetDevice();
    VkResult result = vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, &commandPool);
    if(result != VK_SUCCESS) return false;

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
        if(result != VK_SUCCESS) return false;

        VkCommandBufferBeginInfo commandBufferBeginInfo = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                .pNext = nullptr,
                .flags = 0,
                .pInheritanceInfo = nullptr,
        };
        result = vkBeginCommandBuffer(commandBuffers[i], &commandBufferBeginInfo);
        if(result != VK_SUCCESS) return false;

        // Image Layout
        setImageLayout(commandBuffers[i], frameBuffer.GetViews()[i],
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
        auto cmdBuffer = renderInfo.GetCmdBuffer()[i];
        vkCmdBeginRenderPass(cmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        setImageLayout(cmdBuffer, frameBuffer.GetImages()[i],
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);
        vkEndCommandBuffer(cmdBuffer);
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
