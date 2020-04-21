#include "VulkanCommandList.h"
#include "VulkanDevice.h"
#include "Log.h"

bool VulkanCommandList::Create(revDevice* device, revGraphicsPipeline* pipeline, revGraphicsCommandAllocator* allocator)
{
    (void)pipeline;

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