#include <VulkanFrameBuffer.h>
#include "VulkanPipelineState.h"

bool VulkanPipelineState::Create(revDevice* device, const revMaterial& material, const VulkanDescriptorSetLayout& descriptorSetLayout, const revRect& viewportRect, const revRect& scissorRect)
{
    this->device = device;
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.pNext = nullptr;
    pipelineLayoutCreateInfo.setLayoutCount = descriptorSetLayout.GetDescriptorSetCount();
    pipelineLayoutCreateInfo.pSetLayouts = descriptorSetLayout.GetDescriptorSet();
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

    VkResult result = vkCreatePipelineLayout(device->GetDevice(), &pipelineLayoutCreateInfo, nullptr, &pipelineLayout);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    // Viewport and Scissor
    VkViewport viewport;
    viewport.height = viewportRect.h;
    viewport.width = viewportRect.w;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    viewport.x = 0;
    viewport.y = 0;

    VkRect2D scissor;
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    scissor.extent.height = scissorRect.h;
    scissor.extent.width = scissorRect.w;

    VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo = {};
    pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    pipelineViewportStateCreateInfo.pNext = nullptr;
    pipelineViewportStateCreateInfo.viewportCount = 1;
    pipelineViewportStateCreateInfo.pViewports = &viewport;
    pipelineViewportStateCreateInfo.scissorCount = 1;
    pipelineViewportStateCreateInfo.pScissors = &scissor;

    // Shader
    const char* shaderEntry = "main";
    VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo[2];
    for(uint32 i = 0; i <= static_cast<uint32>(SHADER_TYPE::FRAGMENT); ++i){
        pipelineShaderStageCreateInfo[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        pipelineShaderStageCreateInfo[i].pNext = nullptr;
        pipelineShaderStageCreateInfo[i].module = material.GetShader(static_cast<SHADER_TYPE>(i))->GetHandle();
        pipelineShaderStageCreateInfo[i].pSpecializationInfo = nullptr;
        pipelineShaderStageCreateInfo[i].flags = 0;
        pipelineShaderStageCreateInfo[i].pName = shaderEntry;
    }

    const auto& blendState = material.GetBlendState();

    // Sampler
    VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo = {};
    pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    pipelineMultisampleStateCreateInfo.pNext = nullptr;
    pipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    pipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
    pipelineMultisampleStateCreateInfo.minSampleShading = 0;
    pipelineMultisampleStateCreateInfo.pSampleMask = nullptr;
    pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = blendState.isEbaleAlphaToCoverage() ? VK_TRUE : VK_FALSE;
    pipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

    // TODO: multiple sampler setting
    VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState;
    pipelineColorBlendAttachmentState.colorWriteMask = static_cast<VkColorComponentFlags>(blendState.GetWriteColorMask());
    pipelineColorBlendAttachmentState.blendEnable = static_cast<VkBool32>(blendState.isEnableBlend());
    pipelineColorBlendAttachmentState.colorBlendOp = static_cast<VkBlendOp>(blendState.GetBlendOpColor());
    pipelineColorBlendAttachmentState.srcColorBlendFactor = static_cast<VkBlendFactor>(blendState.GetBlendFactorSrcColor());
    pipelineColorBlendAttachmentState.dstColorBlendFactor = static_cast<VkBlendFactor>(blendState.GetBlendFactorDstColor());
    pipelineColorBlendAttachmentState.alphaBlendOp = static_cast<VkBlendOp>(blendState.GetBlendOpAlpha());;
    pipelineColorBlendAttachmentState.srcAlphaBlendFactor = static_cast<VkBlendFactor>(blendState.GetBlendFactorSrcAlpha());
    pipelineColorBlendAttachmentState.dstAlphaBlendFactor = static_cast<VkBlendFactor>(blendState.GetBlendFactorDstAlpha());
    pipelineColorBlendAttachmentState.dstAlphaBlendFactor = static_cast<VkBlendFactor>(blendState.GetBlendFactorDstAlpha());

    VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo = {};
    pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pipelineColorBlendStateCreateInfo.pNext = nullptr;
    pipelineColorBlendStateCreateInfo.logicOpEnable = blendState.isEnabledLogicOp() ? VK_FALSE : VK_TRUE;
    pipelineColorBlendStateCreateInfo.logicOp = ConvertToVKLogicOp(blendState.GetLogicOp());
    pipelineColorBlendStateCreateInfo.attachmentCount = 1;
    pipelineColorBlendStateCreateInfo.pAttachments = &pipelineColorBlendAttachmentState;
    pipelineColorBlendStateCreateInfo.flags = 0;

    // Rasterizer
    const auto& rasterization = material.GetRasterization();
    VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo = {};
    pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    pipelineRasterizationStateCreateInfo.pNext = nullptr;
    pipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
    pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
    pipelineRasterizationStateCreateInfo.polygonMode = ConvertToVKFillMode(rasterization.GetPolygonMode());
    pipelineRasterizationStateCreateInfo.cullMode = ConvertToVKCullMode(rasterization.GetCullMode());
    pipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    pipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
    pipelineRasterizationStateCreateInfo.lineWidth = 1;

    // input assembler
    VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo = {};
    pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    pipelineInputAssemblyStateCreateInfo.pNext = nullptr;
    pipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

    revArray<VkVertexInputBindingDescription> vertexInputBindings;
    revArray<VkVertexInputAttributeDescription> vertexInputAttributes;
    const revShader* vertexShader = material.GetShader(SHADER_TYPE::VERTX);
    if(vertexShader != nullptr) {
        const auto& attributes = vertexShader->GetAttributes();
        uint32 attributeLength = attributes.size();

        vertexInputBindings.resize(attributeLength);
        vertexInputAttributes.resize(attributeLength);

        uint32 offset = 0;
        for (uint32 i = 0; i < attributeLength; ++i) {
            vertexInputBindings[i].binding = attributes[i].GetBinding();
            vertexInputBindings[i].stride = GRAPHICS_SEMANTICS[static_cast<uint32>(attributes[i].GetInputElementType())].sizeOfBytes;
            vertexInputBindings[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX,

            vertexInputAttributes[i].binding = attributes[i].GetBinding();
            vertexInputAttributes[i].location = attributes[i].GetLocation();
            vertexInputAttributes[i].format = ConvertToVKFormat(attributes[i].GetForamt());
            vertexInputAttributes[i].offset = attributes[i].GetOffset();
        }
    }

    VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
    vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputStateCreateInfo.pNext = nullptr;
    vertexInputStateCreateInfo.vertexBindingDescriptionCount = vertexInputBindings.size();
    vertexInputStateCreateInfo.pVertexBindingDescriptions = vertexInputBindings.data();
    vertexInputStateCreateInfo.vertexAttributeDescriptionCount = vertexInputAttributes.size();
    vertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexInputAttributes.data();

    // render pass
    // TODO: create render pass from shader meta data.
    VkAttachmentDescription attachmentDescription = {};
    attachmentDescription.format = VK_FORMAT_R8G8B8A8_UNORM;
    attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
    attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference attachmentReference;
    attachmentReference.attachment = 0;
    attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpassDescription;
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.flags = 0;
    subpassDescription.inputAttachmentCount = 0;
    subpassDescription.pInputAttachments = nullptr;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = nullptr;
    subpassDescription.pResolveAttachments = nullptr;
    subpassDescription.pDepthStencilAttachment = nullptr;
    subpassDescription.preserveAttachmentCount = 0;
    subpassDescription.pPreserveAttachments = nullptr;

    VkRenderPassCreateInfo renderPassCreateInfo ={};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.pNext = nullptr;
    renderPassCreateInfo.attachmentCount = 1;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpassDescription;
    renderPassCreateInfo.dependencyCount = 0;
    renderPassCreateInfo.pDependencies = nullptr;
    result = vkCreateRenderPass(device->GetDevice(), &renderPassCreateInfo, nullptr, &renderPass);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    // pipeline cache
    VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
            .pNext = nullptr,
            .initialDataSize = 0,
            .pInitialData = nullptr,
            .flags = 0,
    };

    result = vkCreatePipelineCache(device->GetDevice(), &pipelineCacheCreateInfo, nullptr, &pipelineCache);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    // Pipeline
    VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
    graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphicsPipelineCreateInfo.pNext = nullptr;
    graphicsPipelineCreateInfo.flags = 0;
    graphicsPipelineCreateInfo.stageCount = 2;
    graphicsPipelineCreateInfo.pStages = pipelineShaderStageCreateInfo;
    graphicsPipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
    graphicsPipelineCreateInfo.pInputAssemblyState = &pipelineInputAssemblyStateCreateInfo;
    graphicsPipelineCreateInfo.pTessellationState = nullptr;
    graphicsPipelineCreateInfo.pViewportState = &pipelineViewportStateCreateInfo;
    graphicsPipelineCreateInfo.pRasterizationState = &pipelineRasterizationStateCreateInfo;
    graphicsPipelineCreateInfo.pMultisampleState = &pipelineMultisampleStateCreateInfo;
    graphicsPipelineCreateInfo.pDepthStencilState = nullptr; // TODO: create depth stencil
    graphicsPipelineCreateInfo.pColorBlendState = &pipelineColorBlendStateCreateInfo;
    graphicsPipelineCreateInfo.pDynamicState = nullptr;
    graphicsPipelineCreateInfo.layout = pipelineLayout;
    graphicsPipelineCreateInfo.renderPass = renderPass;
    graphicsPipelineCreateInfo.subpass = 0;
    graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    graphicsPipelineCreateInfo.basePipelineIndex = 0;

    result = vkCreateGraphicsPipelines(device->GetDevice(), pipelineCache, 1, &graphicsPipelineCreateInfo, nullptr, &pipelineState);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

     return true;
}

void VulkanPipelineState::Apply(VulkanCommandList& commandList, const VulkanFrameBuffer& frameBuffer, const revColor& clearColor)
{
    // needs Image layout setting?
    VkRenderPassBeginInfo renderPassBeginInfo;
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = nullptr;
    renderPassBeginInfo.framebuffer = ;
    renderPassBeginInfo.renderPass = renderPass;

    vkCmdBindPipeline(commandList.GetList(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineState);
}

void VulkanPipelineState::Destroy()
{
    auto& vkDevice = device->GetDevice();
    vkDestroyPipeline(vkDevice, pipelineState, nullptr);
    vkDestroyPipelineCache(vkDevice, pipelineCache, nullptr);
    vkDestroyRenderPass(vkDevice, renderPass, nullptr);
    vkDestroyPipelineLayout(vkDevice, pipelineLayout, nullptr);

}
