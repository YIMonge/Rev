#include "VulkanRenderInfo.h"

#ifdef _USE_VULKAN

bool VulkanRenderInfo::Create(const VulkanDeviceContext &deviceContext, const VulkanSwapChain& swapChain)
{
    VkAttachmentDescription attachmentDescription {
        .format = swapChain.GetFormat(),
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    };

    VkAttachmentReference colorReference {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

    VkSubpassDescription subpassDescription {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .flags = 0,
        .inputAttachmentCount = 0,
        .pInputAttachments = nullptr,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorReference,
        .pResolveAttachments = nullptr,
        .pDepthStencilAttachment = nullptr,
        .preserveAttachmentCount = 0,
        .pPreserveAttachments = nullptr,
    };

    VkRenderPassCreateInfo renderPassCreateInfo {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext = nullptr,
        .attachmentCount = 1,
        .pAttachments = &attachmentDescription,
        .subpassCount = 1,
        .pSubpasses = &subpassDescription,
        .dependencyCount = 0,
        .pDependencies = nullptr,
    };

    VkDevice  device = deviceContext.GetDevice();
    VkResult result = vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &renderPass);
    if(result != VK_SUCCESS) return false;

    // create fence and semaphore
    VkFenceCreateInfo fenceCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
    };
    result = vkCreateFence(device, &fenceCreateInfo, nullptr, &fence);
    if(result != VK_SUCCESS) return false;

    VkSemaphoreCreateInfo semaphoreCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
    };
    result = vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &semaphore);
    if(result != VK_SUCCESS) return false;

    return true;
}

void VulkanRenderInfo::Destroy(const VulkanDeviceContext& deviceContext)
{
    VkDevice device = deviceContext.GetDevice();
    if(cmdBuffer != nullptr) {
        vkFreeCommandBuffers(device, cmdPool, cmdBufferLength, cmdBuffer);
        delete[] cmdBuffer;
    }
    vkDestroyCommandPool(device, cmdPool, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);
}

#endif
