#include <VulkanTexture.h>
#include "VulkanRenderInfo.h"
#include "Log.h"
#ifdef _USE_VULKAN

VulkanRenderInfo::VulkanRenderInfo()
{
    memset(&pipelineLayout, 0, sizeof(pipelineLayout));
    memset(&pipelineCache, 0, sizeof(pipelineCache));
    memset(&pipeline, 0, sizeof(pipeline));
}

VulkanRenderInfo::~VulkanRenderInfo()
{

}

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

    const VkDevice&  device = deviceContext.GetDevice();
    VkResult result = vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &renderPass);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    // create fence and semaphore
    VkFenceCreateInfo fenceCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
    };
    result = vkCreateFence(device, &fenceCreateInfo, nullptr, &fence);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    VkSemaphoreCreateInfo semaphoreCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
    };
    result = vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &semaphore);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    return true;
}

// NOTICE: this is old function. it will be obsolete after wrote create from material.
// TODO: determine each states by vertx and fragment shader. ex, input assembler is chosen by vertex shader, blend state is decided by fragment shader(material).
// TODO: Material should have vertex/fragment shader, input assembler, blend and depth/stencil state. create pipeline by Material.
// TODO: if use multiple vertex buffer, like first vertex last valying attributes must create multiple binding and attribute info.
bool VulkanRenderInfo::CreatePipeline(const VulkanDeviceContext& deviceContext, const VulkanSwapChain& swapChain, const VulkanShader& vertexShader, const VulkanShader& fragmentShader)
{
    const VkDevice& device = deviceContext.GetDevice();

    // Texture settings
    VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1, // for test
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
        .pImmutableSamplers = nullptr,
    };
    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .bindingCount = 1,
        .pBindings = &descriptorSetLayoutBinding,
    };
    VkResult result = vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCreateInfo, nullptr, &descriptorSetLayout);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    //---------------------------------------------------------------
    // TODO: cached pipeline and load pipeline if pilinecache has alredy existed.
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .setLayoutCount = 1,
        .pSetLayouts = &descriptorSetLayout,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = nullptr,
    };

    result = vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    VkPipelineShaderStageCreateInfo shaderStageCreateInfo[2];
    shaderStageCreateInfo[0] = vertexShader.getShaderStageCreateInfo();
    shaderStageCreateInfo[1] = fragmentShader.getShaderStageCreateInfo();

    VkViewport viewport = {
            .height = static_cast<float>(swapChain.GetDisplaySize().height),
            .width = static_cast<float>(swapChain.GetDisplaySize().width),
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
            .x = 0,
            .y = 0,
    };
    VkRect2D scissor = {
            .extent = swapChain.GetDisplaySize(),
            .offset = {
                    .x = 0,
                    .y = 0,
            },
    };
    VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .pNext = nullptr,
            .viewportCount = 1,
            .pViewports = &viewport,
            .scissorCount = 1,
            .pScissors = &scissor,
    };

    // TODO: will do more research
    VkSampleMask  sampleMask = ~0u;
    VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .pNext = nullptr,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .sampleShadingEnable = VK_FALSE,
            .minSampleShading = 0,
            .pSampleMask = &sampleMask,
            .alphaToCoverageEnable = VK_FALSE,
            .alphaToOneEnable = VK_FALSE,
    };

    // color blend state
    VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {
            .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                    VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
            .blendEnable = VK_FALSE,
    };
    VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .pNext = nullptr,
            .logicOpEnable = VK_FALSE,
            .logicOp = VK_LOGIC_OP_COPY,
            .attachmentCount = 1,
            .pAttachments = &colorBlendAttachmentState,
            .flags = 0,
    };

    // rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .pNext = nullptr,
            .depthClampEnable = VK_FALSE,
            .rasterizerDiscardEnable = VK_FALSE,
            .polygonMode = VK_POLYGON_MODE_FILL,
            .cullMode = VK_CULL_MODE_NONE,
            .frontFace = VK_FRONT_FACE_CLOCKWISE,
            .depthBiasEnable = VK_FALSE,
            .lineWidth = 1,
    };

    // input assembler state
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .pNext = nullptr,
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE,
    };

    VkVertexInputBindingDescription vertexInputBindingDescription = {
            .binding = 0,
            .stride = 3 * sizeof(float),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
    };
    VkVertexInputAttributeDescription vertexInputAttributeDescription = {
            .binding = 0,
            .location = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .offset = 0,
    };
    VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext = nullptr,
            .vertexBindingDescriptionCount = 1,
            .pVertexBindingDescriptions = &vertexInputBindingDescription,
            .vertexAttributeDescriptionCount = 1,
            .pVertexAttributeDescriptions = &vertexInputAttributeDescription,
    };


    // pipeline cache
    VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
            .pNext = nullptr,
            .initialDataSize = 0,
            .pInitialData = nullptr,
            .flags = 0,
    };

    result = vkCreatePipelineCache(device, &pipelineCacheCreateInfo, nullptr, &pipelineCache);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    // create the pipeline
    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stageCount = 2,
            .pStages = shaderStageCreateInfo,
            .pVertexInputState = &vertexInputStateCreateInfo,
            .pInputAssemblyState = &inputAssemblyStateCreateInfo,
            .pTessellationState = nullptr,
            .pViewportState = &pipelineViewportStateCreateInfo,
            .pRasterizationState = &rasterizationStateCreateInfo,
            .pMultisampleState = &multisampleStateCreateInfo,
            .pDepthStencilState = nullptr,
            .pColorBlendState = &colorBlendStateCreateInfo,
            .pDynamicState = nullptr,
            .layout = pipelineLayout,
            .renderPass = renderPass,
            .subpass = 0,
            .basePipelineHandle = VK_NULL_HANDLE,
            .basePipelineIndex = 0,
    };

    result = vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineCreateInfo, nullptr, &pipeline);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    return true;
}


bool VulkanRenderInfo::CreatePipeline(const VulkanDeviceContext& deviceContext, const VulkanSwapChain& swapChain, const revMaterial* material)
{
    if(!material) return false;

    const revGraphicsDevice& device = deviceContext.GetDevice();

    // Texture
    auto textures = material->GetTextures();
    VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
        .pImmutableSamplers = nullptr,
    };
    descriptorSetLayoutBinding.descriptorCount = textures.size();

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .bindingCount = 1,
            .pBindings = &descriptorSetLayoutBinding,
    };
    VkResult result = vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCreateInfo, nullptr, &descriptorSetLayout);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .setLayoutCount = 1,
            .pSetLayouts = &descriptorSetLayout,
            .pushConstantRangeCount = 0,
            .pPushConstantRanges = nullptr,
    };

    result = vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    // Viewport and Scissor
    VkViewport viewport = {
            .height = static_cast<float>(swapChain.GetDisplaySize().height),
            .width = static_cast<float>(swapChain.GetDisplaySize().width),
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
            .x = 0,
            .y = 0,
    };
    VkRect2D scissor = {
            .extent = swapChain.GetDisplaySize(),
            .offset = {
                    .x = 0,
                    .y = 0,
            },
    };
    VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .pNext = nullptr,
            .viewportCount = 1,
            .pViewports = &viewport,
            .scissorCount = 1,
            .pScissors = &scissor,
    };

    // Shader
    const char* shaderEntry = "main";
    VkPipelineShaderStageCreateInfo shaderStageCreateInfo[2];
    for(uint32 i = 0; i < static_cast<uint32>(SHADER_TYPE::MAX_NUM); ++i) {
        shaderStageCreateInfo[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageCreateInfo[i].pNext = nullptr;
        shaderStageCreateInfo[i].module = material->GetShader(static_cast<SHADER_TYPE>(i))->GetHandle();
        shaderStageCreateInfo[i].pSpecializationInfo = nullptr;
        shaderStageCreateInfo[i].flags = 0;
        shaderStageCreateInfo[i].pName = shaderEntry;
    }

    // Multi sample
    //VkSampleMask sampleMask = 0;
    VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .pNext = nullptr,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT, // TODO: test other prams
        .sampleShadingEnable = VK_FALSE,
        .minSampleShading = 0,
        .pSampleMask = nullptr,
        .alphaToCoverageEnable = VK_FALSE,              // TODO: by material
        .alphaToOneEnable = VK_FALSE,
    };

    // Blend state
    auto blendState = material->GetBlendState();
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

    VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .pNext = nullptr,
        .logicOpEnable = VK_FALSE,
        .logicOp = VK_LOGIC_OP_COPY,
        .attachmentCount = 1,
        .pAttachments = &pipelineColorBlendAttachmentState,
        .flags = 0,
    };

    // rasterization
    auto rasterization = material->GetRasterization();
    VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext = nullptr,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = static_cast<VkPolygonMode>(rasterization.GetPolygonMode()),
        .cullMode = static_cast<VkCullModeFlagBits>(rasterization.GetCullMode()),
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,
        .lineWidth = 1,
    };

    // input assembler
    VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = nullptr,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE,
    };

    const revShader* vertexShader = material->GetShader(SHADER_TYPE::VERTX);
    revArray<REV_GRAPHICS_BUFFER_FOMAT_FLAG > bufferFormats;
    if(vertexShader) {
        bufferFormats = vertexShader->GetFormats();
    }
    revArray<VkVertexInputAttributeDescription> vertexInputAttributes(bufferFormats.size());
    for(uint32 i = 0; i < bufferFormats.size(); ++i){
        //vertexInputAttributes[i].binding
        // TODO: binding
    }


    return true;
}



// TODO: array of texture
bool VulkanRenderInfo::CreateDescriptorSet(const VulkanDeviceContext& deviceContext, VulkanTexture& texture)
{
    const VkDevice& device = deviceContext.GetDevice();
    VkDescriptorPoolSize descriptorPoolSize = {
        .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1, // if texture count higher than 2 the param should texture count.
    };
    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .pNext = nullptr,
        .maxSets = 1,
        .poolSizeCount = 1,
        .pPoolSizes = &descriptorPoolSize,
    };
    VkResult result = vkCreateDescriptorPool(device, &descriptorPoolCreateInfo, nullptr, &descriptorPool);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = nullptr,
        .descriptorPool = descriptorPool,
        .descriptorSetCount = 1,
        .pSetLayouts = &descriptorSetLayout,
    };
    result = vkAllocateDescriptorSets(device, &descriptorSetAllocateInfo, &descriptorSet);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    // TODO: array of texture
    VkDescriptorImageInfo descriptorImageInfo = texture.GetDescriptorImageInfo();

    VkWriteDescriptorSet writeDescriptorSet = {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .pNext = nullptr,
        .dstSet = descriptorSet,
        .dstBinding = 0,
        .dstArrayElement = 0,
        .descriptorCount = 1, // texture count
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .pImageInfo = &descriptorImageInfo, // if use multi textures, the param should be array
        .pBufferInfo = nullptr,
        .pTexelBufferView = nullptr,
    };
    vkUpdateDescriptorSets(device, 1, &writeDescriptorSet, 0, nullptr);

    return true;
}

void VulkanRenderInfo::Destroy(const VulkanDeviceContext& deviceContext)
{
    VkDevice device = deviceContext.GetDevice();
    vkDestroyRenderPass(device, renderPass, nullptr);
}

#endif
