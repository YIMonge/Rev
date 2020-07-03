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
	if (!CreateFence()) {
		NATIVE_LOGE("Vulkan command list create failed. file:%s, line:%d", __FILE__, __LINE__);
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
	triangleVertexBuffer = new VulkanVertexBuffer(&device);
	triangleVertexBuffer->Create(triangleVertices, sizeof(float) * 5, 3);

	constantBuffer = new VulkanConstantBuffer(&device);
    constantBuffer->Create(nullptr, sizeof(revVector4), 1024, revGraphicsBuffer::USAGE::DYNAMIC);

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

	renderPass.Create(&device, &swapChain, mat);
	swapChain.CreateFrameBuffer(renderPass);
    
    device.GetGlobalCommandList().Close();
    ExecuteCommand(device.GetGlobalCommandList());
    // TODO: wait for fence move to device from swapchain.
	WaitForFence();
	ResetFence();

    //-----------------------------------------------------------------------------

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

    
#if _TEX && !_CBUFFER
	VulkanDescriptorSet::Chunk chunk = descriptorSet.Allocation(1, 0 + GetDescriptorBindingOffset(DESCRIPTOR_TYPE::TEXTURE_SHADER_RESOURCE_VIEW));
    textureView.Create(&device, texture, sampler, chunk);
#endif

#if _CBUFFER
	VulkanDescriptorSet::Chunk chunk = descriptorSet.Allocation(1, 0);
	constantBufferView.Create(&device, *constantBuffer, chunk);
#if _TEX
	chunk = descriptorSet.Allocation(1, 0 + GetDescriptorBindingOffset(DESCRIPTOR_TYPE::TEXTURE_SHADER_RESOURCE_VIEW));
    textureView.Create(&device, texture, sampler, chunk);
	chunk = descriptorSet.Allocation(1, 0 + GetDescriptorBindingOffset(DESCRIPTOR_TYPE::SAMPLER));
	samplerView.Create(&device, sampler, chunk);
#endif
#endif

    descriptorSet.Update();

    // Make Draw Command
    auto& commandLists = device.GetCommandLists();
    for(uint32 i = 0; i < commandLists.size(); ++i){
        auto& commandList = commandLists[i];
        commandList.Open();
        auto& frameBuffer = swapChain.GetFrameBuffer();
		/*
        commandList.AddTransitionBarrier(frameBuffer.GetFrameBufferImage(i),
                                         VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                         VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
			*/							
        renderPass.Begin(commandList, frameBuffer.GetFrameBuffer(i), clearValue, swapChain.GetDisplaySize());
        pipelineState.Apply(commandList);
        descriptorSet.Apply(commandList, pipelineState.GetPipelineLayout());

        VkDeviceSize offset = 0;
        vkCmdBindVertexBuffers(commandList.GetList(), 0, 1, triangleVertexBuffer->GetHandlePtr(), &offset);
        vkCmdDraw(commandList.GetList(), 3, 1, 0, 0);

        renderPass.End(commandList);
		/*
        commandList.AddTransitionBarrier(frameBuffer.GetFrameBufferImage(i),
                                         VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);
			*/							 
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
    constantBuffer->Update(cbufferOffset.data, sizeof(revVector4));

    auto& commandList = device.GetCommandLists()[swapChain.GetCurrentFrameIndex()];
     ExecuteCommand(commandList, true);
	 WaitForFence();
     swapChain.Present();
	 ResetFence();
	 commandList.ReleaseResource();
}

void VulkanRenderer::ExecuteCommand(revArray<VulkanCommandList>& lists, bool needSemaphore)
{
    uint32 length = static_cast<uint32>(lists.size());
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
    VkResult result = vkQueueSubmit(device.GetQueue(), 1, &submitInfo, fence);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
    }
}

void VulkanRenderer::ExecuteCommand(VulkanCommandList& list, bool needSemaphore)
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
    VkResult result = vkQueueSubmit(device.GetQueue(), 1, &submitInfo, fence);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
    }
}

bool VulkanRenderer::CreateFence()
{
	auto device = this->device.GetDevice();
	// create fence and semaphore
	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.pNext = nullptr;
	fenceCreateInfo.flags = 0;

	VkResult result = vkCreateFence(device, &fenceCreateInfo, nullptr, &fence);
	if (result != VK_SUCCESS) {
		NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
		return false;
	}
	ResetFence();
	return true;
}

void VulkanRenderer::WaitForFence()
{
	VkResult result = vkWaitForFences(device.GetDevice(), 1, &fence, VK_TRUE, 100000000);
	if (result != VK_SUCCESS) {
		NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
	}
}

void VulkanRenderer::ResetFence()
{
	VkResult result = vkResetFences(device.GetDevice(), 1, &fence);
	if (result != VK_SUCCESS) {
		NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
		return;
	}
}

#endif
