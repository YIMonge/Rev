#include "VkRenderInfo.h"

bool VkRenderInfo::Create(const VkDeviceContext &deviceContext, const VkSwapChain& swapChain)
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

    return vkCreateRenderPass(deviceContext.GetDevice(), &renderPassCreateInfo, nullptr, &renderPass) == VK_SUCCESS;
}

void VkRenderInfo::Destroy(const VkDeviceContext& deviceContext)
{
    VkDevice device = deviceContext.GetDevice();
    if(cmdBuffer != nullptr) {
        vkFreeCommandBuffers(device, cmdPool, cmdBufferLength, cmdBuffer);
        delete[] cmdBuffer;
    }
    vkDestroyCommandPool(device, cmdPool, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);
}