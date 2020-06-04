#ifdef _USE_VULKAN
#include <revColor.h>
#include "VulkanRenderer.h"
#include "Log.h"

// TEST
#include "VulkanTexture.h"
#include "VulkanShader.h"

VulkanRenderer::VulkanRenderer()
{
    clearValue = revColor::blue;
}


VulkanRenderer::~VulkanRenderer()
{
}


void VulkanRenderer::StartUp(Window* window, const GraphicsDesc& desc)
{
    if(!device.Create(*window)) {
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
    constantBuffer.Create(&device, nullptr, sizeof(revVector4), 1024, revGraphicsBuffer::USAGE::DYNAMIC);

    VulkanShader shader[2];
    shader[0].LoadFromFile(device, "cbuffer_vert.spv", SHADER_TYPE::VERTX);
    shader[1].LoadFromFile(device, "cbuffer_frag.spv", SHADER_TYPE::FRAGMENT);

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

    renderPass.Create(&device, mat);
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
    descriptorSet.Create(&device, descriptorSetLayout, 4, true);
    textureView.Create(&device, texture, sampler, &descriptorSet);



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
    uint32 length = lists.size();
    revArray<revGraphicsCommandBuffer> commandlists(lists.size());
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
