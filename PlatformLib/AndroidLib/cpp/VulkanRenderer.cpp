#include <revColor.h>
#include "VulkanRenderer.h"
#include "Log.h" // AndroidLib doesn't depend on rev
#ifdef _USE_VULKAN
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
    const float triangleVertices[] = {
            // vertex         , texcoord
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
             0.0f,  1.0f, 0.0f, 0.5f, 1.0f,
    };
    triangleVertexBuffer.Create(device, triangleVertices, sizeof(triangleVertices), 3);

    // Load shader and material
    VulkanShader shader[2];
    shader[0].LoadFromFile(device, "texture_vert.spv", SHADER_TYPE::VERTX);
    shader[1].LoadFromFile(device, "texture_frag.spv", SHADER_TYPE::FRAGMENT);

    mat.SetShader(SHADER_TYPE::VERTX, &shader[0]);
    mat.SetShader(SHADER_TYPE::FRAGMENT, &shader[1]);

    VulkanTexture texture;
    texture.LoadFromFile(&device, "sample_tex.png");
    sampler.Create(&device, texture);

    // create pipeline
    revDescriptorBindingDesc descriptorBindingDesc;
    descriptorBindingDesc.AddMaterial(mat);
    descriptorSetLayout.Create(&device, descriptorBindingDesc);
    descriptorSet.Create(&device, descriptorSetLayout, 4, true);
    textureView.Create(&device, texture, sampler, &descriptorSet);
    pipelineState.Create(&device, mat, descriptorSetLayout, swapChain.GetDisplaySize(), swapChain.GetDisplaySize());
    swapChain.CreateFrameBuffer(pipelineState);

    // Make Draw commadn
    auto& commandLists = device.GetCommandLists();
    for(uint32 i = 0; i < commandLists.size(); ++i){
        auto& commandList = commandLists[i];
        commandList.Open();
        swapChain.PrepareRendering(commandList, i);

        pipelineState.BeginRenderPass(commandList, swapChain.GetCurrentFrameBuffer(), clearValue);
        descriptorSet.Apply(commandList, pipelineState.GetPipelineLayout());

        triangleVertexBuffer.Apply(commandList);
        vkCmdDraw(commandList.GetList(), 3, 1, 0, 0);

        pipelineState.EndRenderPass(commandList);
        swapChain.EndRendering(commandList, i);

        commandList.Close();
//        ExecuteCommand(commandList);
 //       swapChain.WaitForPreviousFrame();
  //      swapChain.Present();
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
    auto& commandList = device.GetCommandLists()[swapChain.GetCurrentFrameIndex()];
     ExecuteCommand(commandList);
     swapChain.WaitForPreviousFrame();
     swapChain.Present();
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
            .pWaitSemaphores = swapChain.GetSemaphore(),
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
            .pWaitSemaphores = swapChain.GetSemaphore(),
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
