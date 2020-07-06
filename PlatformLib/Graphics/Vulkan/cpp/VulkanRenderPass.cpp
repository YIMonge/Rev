#ifdef _USE_VULKAN

#include "VulkanRenderPass.h"
#include "VulkanSwapChain.h"

VulkanRenderPass::VulkanRenderPass()
{
}

VulkanRenderPass::~VulkanRenderPass()
{
}

bool VulkanRenderPass::Create(revDevice* device, VulkanSwapChain* swapChain, const revMaterial& material)
{
    this->device = device;

	// render target 
    VkAttachmentDescription colorAttachmentDescription = {};
    colorAttachmentDescription.format = ConvertToVKFormat(swapChain->GetFormat());
    colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentReference;
    colorAttachmentReference.attachment = 0;
    colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	// depth 
	const VulkanDepthBuffer& depthBuffer = swapChain->GetDepthBuffer();
	VkAttachmentDescription depthAttachmentDescription = {};
	depthAttachmentDescription.format = depthBuffer.GetVulkanFormat();
	depthAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentReference;
	depthAttachmentReference.attachment = 1;
	depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;


    VkSubpassDescription subpassDescription;
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.flags = 0;
    subpassDescription.inputAttachmentCount = 0;
    subpassDescription.pInputAttachments = nullptr;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorAttachmentReference;
    subpassDescription.pResolveAttachments = nullptr;
	subpassDescription.pDepthStencilAttachment = &depthAttachmentReference;
    subpassDescription.preserveAttachmentCount = 0;
    subpassDescription.pPreserveAttachments = nullptr;


	VkAttachmentDescription descriptions[2] = {
		colorAttachmentDescription,
		depthAttachmentDescription
	};
    VkRenderPassCreateInfo renderPassCreateInfo = {};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.pNext = nullptr;
    renderPassCreateInfo.attachmentCount = 2;
    renderPassCreateInfo.pAttachments = descriptions;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpassDescription;
    renderPassCreateInfo.dependencyCount = 0;
    renderPassCreateInfo.pDependencies = nullptr;
    VkResult result = vkCreateRenderPass(device->GetDevice(), &renderPassCreateInfo, nullptr, &renderPass);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    return true;
}

void VulkanRenderPass::Destroy()
{
    vkDestroyRenderPass(device->GetDevice(), renderPass, nullptr);
}

void VulkanRenderPass::Begin(VulkanCommandList& commandList, const VkFramebuffer& framebuffer, const revColor& clearColor, const revRect& viewport)
{
    VkRenderPassBeginInfo renderPassBeginInfo = {};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = nullptr;
    renderPassBeginInfo.framebuffer = framebuffer;
    renderPassBeginInfo.renderPass = renderPass;
    renderPassBeginInfo.renderArea.offset.x = 0;
    renderPassBeginInfo.renderArea.offset.y = 0;
    renderPassBeginInfo.renderArea.extent.height = viewport.h;
    renderPassBeginInfo.renderArea.extent.width = viewport.w;
    VkClearValue clearValue[2];
    clearValue[0].color.float32[0] = clearColor.data[0];
    clearValue[0].color.float32[1] = clearColor.data[1];
    clearValue[0].color.float32[2] = clearColor.data[2];
    clearValue[0].color.float32[3] = clearColor.data[3];
	
	clearValue[1].depthStencil.depth = 1.0f;
	clearValue[1].depthStencil.stencil = 0;

	renderPassBeginInfo.clearValueCount = 2;
	renderPassBeginInfo.pClearValues = clearValue;

    vkCmdBeginRenderPass(commandList.GetList(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanRenderPass::End(VulkanCommandList& commandList)
{
    vkCmdEndRenderPass(commandList.GetList());
}

#endif
