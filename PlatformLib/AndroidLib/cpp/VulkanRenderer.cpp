#include <revColor.h>
#include "VulkanRenderer.h"
#include "Log.h" // AndroidLib doesn't depend on rev
#ifdef _USE_VULKAN
// TEST
#include "VulkanTexture.h"
#include "VulkanShader.h"

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
    if(!swapChain.Create(&device)) return;
    //if(!renderInfo.Create(&device, swapChain)) return;
    //if(!frameBuffer.Create(device, swapChain, renderInfo)) return;
    //-----------------------------------------------------------------------------------------------
    // TEST CODE
    const float triangleVertices[] = {
            // vertex         , texcoord
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
             0.0f,  1.0f, 0.0f, 0.5f, 1.0f,
    };
    triangleVertexBuffer.Create(device, triangleVertices, sizeof(triangleVertices));

    // Load shader and material
    VulkanShader shader[2];
    shader[0].LoadFromFile(device, "shaders/texture_vert.spv", SHADER_TYPE::VERTX);
    shader[1].LoadFromFile(device, "shaders/texture_frag.spv", SHADER_TYPE::FRAGMENT);

    mat.SetShader(SHADER_TYPE::VERTX, &shader[0]);
    mat.SetShader(SHADER_TYPE::FRAGMENT, &shader[1]);

    VulkanTexture texture;
    texture.LoadFromFile(&device, "sample_tex.png");
    sampler.Create(&device, texture);

    // create pipeline
    revDescriptorBindingDesc descriptorBindingDesc;
    descriptorBindingDesc.AddMaterial(mat);
    descriptorSetLayout.Create(&device, descriptorBindingDesc);
    descriptorSet[0].Create(&device, DESCRIPTOR_HEAP_TYPE::RESOURCE, descriptorSetLayout, 4, true);
    descriptorSet[1].Create(&device, DESCRIPTOR_HEAP_TYPE::SAMPLER, descriptorSetLayout, 4, true);
    textureView.Create(&device, texture, sampler, descriptorSet);

    // create commadn
    auto& commandList = device.GetGlobalCommandList();
    commandList.Open();

}

void VulkanRenderer::ShutDown()
{
    swapChain.Destroy();
    device.Destroy();
}

void VulkanRenderer::Render()
{
    swapChain.WaitForPreviousFrame();
    swapChain.Present();
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

void VulkanRenderer::ExecuteCommand(revArray<revGraphicsCommandList>& lists)
{
    uint32 length = lists.size();
    revArray<revGraphicsCommandBuffer> commandlists(lists.size());
    for (uint32 i = 0; i < length; ++i) {
        commandlists[i] = lists[i].GetList();
    }
    VkPipelineStageFlags  waitStageMask =  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo submitInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &swapChain.GetSemaphore(),
            .pWaitDstStageMask = &waitStageMask,
            .commandBufferCount = length,
            .pCommandBuffers = commandlists.data(),
            .signalSemaphoreCount = 0,
            .pSignalSemaphores = nullptr,
    };
    VkResult result = vkQueueSubmit(device.GetQueue(), 1, &submitInfo, swapChain.GetFence());
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
    }
}

void VulkanRenderer::ExecuteCommand(revGraphicsCommandList& list)
{
    VkPipelineStageFlags  waitStageMask =  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo submitInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &swapChain.GetSemaphore(),
            .pWaitDstStageMask = &waitStageMask,
            .commandBufferCount = 1,
            .pCommandBuffers = &(list.GetList()),
            .signalSemaphoreCount = 0,
            .pSignalSemaphores = nullptr,
    };
    VkResult result = vkQueueSubmit(device.GetQueue(), 1, &submitInfo, swapChain.GetFence());
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
    }
}


#endif
