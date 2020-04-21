#include <revColor.h>
#include "VulkanRenderer.h"
#include "Log.h" // AndroidLib doesn't depend on rev
#ifdef _USE_VULKAN

// TEST
#include "VulkanTexture.h"

VulkanRenderer::VulkanRenderer()
{
    clearValue = {
            .color.float32[0] = 0.3f,
            .color.float32[1] = 0.0f,
            .color.float32[2] = 0.3f,
            .color.float32[3] = 1.0f,
    };
}


VulkanRenderer::~VulkanRenderer()
{
}


void VulkanRenderer::StartUp(Window* window, const GraphicsDesc& desc)
{
    if(!device.Create(*window)) return;
    if(!swapChain.Create(device)) return;
    if(!renderInfo.Create(device, swapChain)) return;
    if(!frameBuffer.Create(device, swapChain, renderInfo)) return;
    //-----------------------------------------------------------------------------------------------
    // TEST CODE
    const float triangleVertices[] = {
            -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    };
    const float triangleTexcoords[] = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            0.5f, 1.0f,
    };
    triangleVertexBuffer.Create(device, triangleVertices, sizeof(triangleVertices));
    triangleTexcoordBuffer.Create(device, triangleTexcoords, sizeof(triangleTexcoords));

    VulkanShader shader[2];
    shader[0].LoadFromFile(device, "shaders/first.vert.spv", SHADER_TYPE::VERTX);
    shader[1].LoadFromFile(device, "shaders/first.frag.spv", SHADER_TYPE::FRAGMENT);


    VulkanTexture texture;
    texture.LoadFromFile(&device, "sample_tex.png");
    if(!renderInfo.CreatePipeline(device, swapChain, shader[0], shader[1])) return;
    if(!renderInfo.CreateDescriptorSet(device, texture)) return;
}

void VulkanRenderer::ShutDown()
{
    frameBuffer.Destroy(device);
    renderInfo.Destroy(device);
    swapChain.Destroy(device);
    device.Destroy();
}

void VulkanRenderer::Render()
{
    uint32 index;
    const revGraphicsDevice& revDevice = device.GetDevice();
    VkResult result;
    result = vkAcquireNextImageKHR(revDevice,
            swapChain.GetSwapChain(),
            UINT64_MAX,
            renderInfo.GetSemaphore(),
            VK_NULL_HANDLE,
            &index);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
    }

    result = vkResetFences(revDevice, 1, &renderInfo.GetFence());
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
            .pCommandBuffers = commandBuffers[index],
            .signalSemaphoreCount = 0,
            .pSignalSemaphores = nullptr,
    };
    result = vkQueueSubmit(device.GetQueue(), 1, &submitInfo, renderInfo.GetFence());
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
    }

    result = vkWaitForFences(revDevice, 1, &renderInfo.GetFence(), VK_TRUE, 100000000);
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
    vkQueuePresentKHR(device.GetQueue(), &presentInfo);
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
                    .baseArrayLayer = 0,
                    .layerCount = 1,
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
        case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
            break;
        default:
            break;
    }

    vkCmdPipelineBarrier(commandBuffer, srcStages, destStages, 0, 0, nullptr, 0, nullptr, 1,
                         &imageMemoryBarrier);
}

#endif
