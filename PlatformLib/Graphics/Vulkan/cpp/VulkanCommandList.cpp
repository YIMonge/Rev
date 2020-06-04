#ifdef _USE_VULKAN

#include "VulkanCommandList.h"
#include "VulkanDevice.h"
#include "Log.h"

bool VulkanCommandList::Create(revDevice* device, revGraphicsPipeline* pipeline, revGraphicsCommandAllocator* allocator)
{
    (void)pipeline;
    this->device = device;

    VkCommandPoolCreateInfo commandPoolCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = 0,
    };

    const revGraphicsDevice& revDevice = device->GetDevice();
    VkResult result;
    if(allocator == nullptr) {
        result = vkCreateCommandPool(revDevice, &commandPoolCreateInfo, nullptr, &commandAllocator);
        if (result != VK_SUCCESS) {
            NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
            return false;
        }
    }
    else commandAllocator = *allocator;

    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                .pNext = nullptr,
                .commandPool = commandAllocator,
                .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                .commandBufferCount = 1,
                };
    result = vkAllocateCommandBuffers(revDevice, &commandBufferAllocateInfo, &commandBuffer);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }



        /*
        // Image Layout
        setImageLayout(commandBuffer, frameBuffer.GetImages()[i],
                       VK_IMAGE_LAYOUT_UNDEFINED,
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

        // TODO: is good in this position? should add command to the buffer by revDraw Command.
        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, renderInfo.GetPipeline());
        VkDeviceSize offset = 0;
        vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, &triangleVertexBuffer.GetHandle(), &offset);
        vkCmdBindVertexBuffers(commandBuffers[i], 1, 1, &triangleTexcoordBuffer.GetHandle(), &offset);
        vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers[i]);
        result = vkEndCommandBuffer(commandBuffers[i]);
        if(result != VK_SUCCESS) {
            NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
            return false;
        }
    }
         */
    return true;
}

void VulkanCommandList::Destroy()
{
    vkFreeCommandBuffers(device->GetDevice(), commandAllocator, 1, &commandBuffer);
}

void VulkanCommandList::Open()
{
    VkCommandBufferBeginInfo commandBufferBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pInheritanceInfo = nullptr,
    };
    VkResult result = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
    }
}

void VulkanCommandList::Close()
{
    VkResult result = vkEndCommandBuffer(commandBuffer);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
    }
}

void VulkanCommandList::AddTransitionBarrier(const VkImage& resource, VkImageLayout before, VkImageLayout after, VkPipelineStageFlags srcStages, VkPipelineStageFlags destStages)
{
    VkImageMemoryBarrier imageMemoryBarrier = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext = nullptr,
            .srcAccessMask = 0,
            .dstAccessMask = 0,
            .oldLayout = before,
            .newLayout = after,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = resource,
            .subresourceRange = {
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel = 0,
                    .levelCount = 1,
                    .baseArrayLayer = 0,
                    .layerCount = 1,
            }
    };

    switch (before) {
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

    switch (after) {
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
        case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
            break;
        default:
            break;
    }

    vkCmdPipelineBarrier(commandBuffer,
                         srcStages, destStages, 0, 0, nullptr, 0, nullptr, 1,
                         &imageMemoryBarrier);
}
#endif
