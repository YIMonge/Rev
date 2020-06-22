#ifdef _USE_VULKAN
#include <revColor.h>
#include "VulkanRenderer.h"
#include "Log.h"

// TEST
#include "VulkanTexture.h"
#include "VulkanShader.h"

#define _CBUFFER 1
#define _TEX 1
#define _TEST 0

VulkanRenderer::VulkanRenderer() :
        initialized(false)
{
    clearValue = revColor::blue;
}


VulkanRenderer::~VulkanRenderer()
{
}


void VulkanRenderer::StartUp(Window* window, const GraphicsDesc& desc)
{
    if(!device.Create(window)) {
        NATIVE_LOGE("Vulkan device create failed. file:%s, line:%d", __FILE__, __LINE__);
        return;
    }
    if(!swapChain.Create(&device)) {
        NATIVE_LOGE("Vulkan swapchain create failed. file:%s, line:%d", __FILE__, __LINE__);
        return;
    }
    // create commandlist for each frame buffer.
    if(!device.CreateCommandList(swapChain.GetLength())) {
        NATIVE_LOGE("Vulkan command list create failed. file:%s, line:%d", __FILE__, __LINE__);
        return;
    }

    //-----------------------------------------------------------------------------------------------
    // TEST CODE
    // Load Assets

    device.GetGlobalCommandList().Open();
    const float triangleVertices[] = {
            // vertex         , texcoord
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
             0.0f,  1.0f, 0.0f, 0.5f, 1.0f,
    };
    triangleVertexBuffer.Create(&device, triangleVertices, sizeof(float) * 5, 3);
    constantBuffer.Create(&device, nullptr, sizeof(revVector4), 1024, revGraphicsBuffer::USAGE::STATIC);

    VulkanShader shader[2];
#if !_CBUFFER && _TEX
    shader[0].LoadFromFile(device, "texture_vert.spv", SHADER_TYPE::VERTX);
    shader[1].LoadFromFile(device, "texture_frag.spv", SHADER_TYPE::FRAGMENT);
#elif _CBUFFER && !_TEX
	shader[0].LoadFromFile(device, "cbuffer_vert.spv", SHADER_TYPE::VERTX);
	shader[1].LoadFromFile(device, "cbuffer_frag.spv", SHADER_TYPE::FRAGMENT);
#elif _CBUFFER && _TEX
    shader[0].LoadFromFile(device, "cbufferTex_vert.spv", SHADER_TYPE::VERTX);
    shader[1].LoadFromFile(device, "cbufferTex_frag.spv", SHADER_TYPE::FRAGMENT);
#endif

    mat.SetShader(SHADER_TYPE::VERTX, &shader[0]);
    mat.SetShader(SHADER_TYPE::FRAGMENT, &shader[1]);

    VulkanTexture texture;
    texture.LoadFromFile(&device, "sample_tex.png");
    sampler.Create(&device, texture);

    
    device.GetGlobalCommandList().Close();
    ExecuteCommand(device.GetGlobalCommandList());
    // TODO: wait for fence move to device from swapchain.
    swapChain.WaitForPreviousFrame();
    
    //-----------------------------------------------------------------------------

    renderPass.Create(&device, &swapChain, mat);
    swapChain.CreateFrameBuffer(renderPass);

    revDescriptorBindingDesc descriptorBindingDesc;
    descriptorBindingDesc.AddMaterial(mat);
    descriptorSetLayout.Create(&device, descriptorBindingDesc);

    PipelineStateDesc pipelineStateDesc = {};
    pipelineStateDesc.material = &mat;
    pipelineStateDesc.renderPass = &renderPass;
    pipelineStateDesc.descriptorSetLayout = &descriptorSetLayout;
    pipelineStateDesc.viewportRect = swapChain.GetDisplaySize();
    pipelineStateDesc.scissorRect = swapChain.GetDisplaySize();
    pipelineState.Create(&device, pipelineStateDesc);

    descriptorPool.Create(&device, 128, 128);
    descriptorSet.Create(&device, descriptorSetLayout, 128, descriptorPool);

    VulkanDescriptorSet::Chunk chunk = descriptorSet.Allocation(1, 0);

#if _TEX && !_CBUFFER
    textureView.Create(&device, texture, sampler, chunk);
#endif

#if _CBUFFER
    constantBufferView.Create(&device, constantBuffer, chunk);
#if _TEX
    chunk = descriptorSet.Allocation(1, 1);
    textureView.Create(&device, texture, sampler, chunk);
#endif
#endif

#if _TEST
    auto descriptorSetHandle = descriptorSet.GetHandle();

    VkDescriptorBufferInfo descriptorBufferInfo = {};
    descriptorBufferInfo.buffer = *constantBuffer.GetHandlePtr();
    descriptorBufferInfo.offset = 0;
    descriptorBufferInfo.range = sizeof(revVector4) * 1024;

    VkWriteDescriptorSet writeDescriptorSet[2];
    writeDescriptorSet[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet[0].pNext = nullptr;
    writeDescriptorSet[0].dstSet = descriptorSetHandle;
    writeDescriptorSet[0].dstBinding = 0;
    writeDescriptorSet[0].dstArrayElement = 0;
    writeDescriptorSet[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeDescriptorSet[0].descriptorCount = 1;
    writeDescriptorSet[0].pImageInfo = nullptr;
    writeDescriptorSet[0].pBufferInfo = &descriptorBufferInfo;
    writeDescriptorSet[0].pTexelBufferView = nullptr;

    //- tex
    VkImageViewCreateInfo imageViewCreateInfo = {};
    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCreateInfo.pNext = nullptr;
    imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCreateInfo.components =
            {
                    VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G,
                    VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A,
            };
    imageViewCreateInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
    imageViewCreateInfo.flags = 0;
    imageViewCreateInfo.format = ConvertToVKFormat(texture.GetFormat());
    imageViewCreateInfo.image = texture.GetHandle();

    VkImageView imageView;
    VkResult result = vkCreateImageView(device.GetDevice(), &imageViewCreateInfo, nullptr, &imageView);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return;
    }

    VkDescriptorImageInfo descriptorImageInfo = {};
    descriptorImageInfo.sampler = sampler.GetHandle();
    descriptorImageInfo.imageView = imageView;
    descriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    writeDescriptorSet[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet[1].pNext = nullptr;
    writeDescriptorSet[1].dstSet = descriptorSetHandle;
    writeDescriptorSet[1].dstBinding = 0;
    writeDescriptorSet[1].dstArrayElement = 0;
    writeDescriptorSet[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescriptorSet[1].descriptorCount = 1;
    writeDescriptorSet[1].pImageInfo = &descriptorImageInfo;
    writeDescriptorSet[1].pBufferInfo = nullptr;
    writeDescriptorSet[1].pTexelBufferView = nullptr;

    vkUpdateDescriptorSets(device.GetDevice(),
                           2,                               // descriptorWriteCount
                           writeDescriptorSet,             // pDescriptorWrites
                           0,                                      // descriptorCopyCount
                           nullptr);
#else
    descriptorSet.Update();
#endif

    // Make Draw Command
    auto& commandLists = device.GetCommandLists();
    for(uint32 i = 0; i < commandLists.size(); ++i){
        auto& commandList = commandLists[i];
        commandList.Open();
        auto& frameBuffer = swapChain.GetFrameBuffer();
        commandList.AddTransitionBarrier(frameBuffer.GetFrameBufferImage(i),
                                         VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                         VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

        renderPass.Begin(commandList, frameBuffer.GetFrameBuffer(i), clearValue, swapChain.GetDisplaySize());
        pipelineState.Apply(commandList);
        descriptorSet.Apply(commandList, pipelineState.GetPipelineLayout());

        VkDeviceSize offset = 0;
        vkCmdBindVertexBuffers(commandList.GetList(), 0, 1, triangleVertexBuffer.GetHandlePtr(), &offset);
        vkCmdDraw(commandList.GetList(), 3, 1, 0, 0);

        renderPass.End(commandList);
        commandList.AddTransitionBarrier(frameBuffer.GetFrameBufferImage(i),
                                         VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);

        commandList.Close();
    }

    initialized = true;
}

void VulkanRenderer::ShutDown()
{
    descriptorSetLayout.Destroy();
    descriptorSet.Destroy();
    pipelineState.Destroy();
    swapChain.Destroy();
    device.Destroy();
}

void VulkanRenderer::Render()
{
    if(!initialized) return;

    cbufferOffset.x += 0.005f;
    if (cbufferOffset.x > 1.25f) cbufferOffset.x = -1.25f;
    constantBuffer.Update(cbufferOffset.data, sizeof(revVector4));

    auto& commandList = device.GetCommandLists()[swapChain.GetCurrentFrameIndex()];
     ExecuteCommand(commandList, true);
     swapChain.WaitForPreviousFrame();
     swapChain.Present();
}

void VulkanRenderer::ExecuteCommand(revArray<revGraphicsCommandList>& lists, bool needSemaphore)
{
    uint32 length = static_cast<uint32>(lists.size());
    revArray<revGraphicsCommandBuffer> commandlists(length);
    for (uint32 i = 0; i < length; ++i) {
        commandlists[i] = lists[i].GetList();
    }
    VkPipelineStageFlags  waitStageMask =  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;
    submitInfo.pWaitDstStageMask = &waitStageMask;
    submitInfo.commandBufferCount = length;
    submitInfo.pCommandBuffers = commandlists.data();
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = nullptr;
    
    submitInfo.waitSemaphoreCount = needSemaphore ? 1 : 0;
    submitInfo.pWaitSemaphores = needSemaphore ? swapChain.GetSemaphore() : nullptr;
    VkResult result = vkQueueSubmit(device.GetQueue(), 1, &submitInfo, swapChain.GetFence());
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
    }
}

void VulkanRenderer::ExecuteCommand(revGraphicsCommandList& list, bool needSemaphore)
{
    VkPipelineStageFlags  waitStageMask =  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;
    submitInfo.pWaitDstStageMask = &waitStageMask;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &(list.GetList());
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = nullptr;
    
    submitInfo.waitSemaphoreCount = needSemaphore ? 1 : 0;
    submitInfo.pWaitSemaphores = needSemaphore ? swapChain.GetSemaphore() : nullptr;
    VkResult result = vkQueueSubmit(device.GetQueue(), 1, &submitInfo, swapChain.GetFence());
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
    }
}


#endif
