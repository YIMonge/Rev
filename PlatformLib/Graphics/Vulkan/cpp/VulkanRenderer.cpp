#ifdef _USE_VULKAN
#include <revColor.h>
#include "VulkanRenderer.h"
#include "Log.h"

// TEST
#include "VulkanTexture.h"
#include "VulkanShader.h"
#include "FbxLoader.h"

struct cbuffer
{
	revMatrix44 world;
	revMatrix44 view;
	revMatrix44 projection;
	revMatrix44 wvp;
};

cbuffer cbufferData;

VulkanRenderer::VulkanRenderer() :
        initialized(false)
{
    clearValue = revColor::BLUE;
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

	// rady for transfer 
	device.GetGlobalCommandList().Open();
	
	if (!swapChain.CreateDepthBuffer()) {
		NATIVE_LOGE("Vulkan depth buffer create failed. file:%s, line:%d", __FILE__, __LINE__);
		return;
	}

    //-----------------------------------------------------------------------------------------------
    // TEST CODE
    // Load Assets
	/*

	cbufferData.view.CreateLookAtMatrixLH(revVector3(0.0f, 0.0f, -10.0f), revVector3(0.0f, 0.0f, 0.0f), revVector3(0.0f, 1.0f, 0.0f));
	cbufferData.projection.CreatePerspectiveMatrixLH(MathUtil::ToRadian(45.0f), window->GetAspectRatio(), 0.001f, 100.0f);
	cbufferData.wvp.Identity();
	cbufferData.wvp = cbufferData.view * cbufferData.projection;

	constantBuffer = new VulkanConstantBuffer(&device);
    constantBuffer->Create(nullptr, sizeof(cbufferData), 1, revGraphicsBuffer::USAGE::DYNAMIC);

    VulkanShader shader[2];
    shader[0].LoadFromFile(device, "model_vert.spv", SHADER_TYPE::VERTX);
    shader[1].LoadFromFile(device, "model_frag.spv", SHADER_TYPE::FRAGMENT);

    mat.SetShader(SHADER_TYPE::VERTX, &shader[0]);
    mat.SetShader(SHADER_TYPE::FRAGMENT, &shader[1]);
	FBXLoader loader;
	loader.LoadFromFile("../../Resources/Models/cube_blender.fbx", &model);
	meshRenderer.SetModel(model);

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
	*/
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

    
	VulkanDescriptorSet::Chunk chunk = descriptorSet.Allocation(1, 0);
	constantBufferView.Create(&device, *constantBuffer, chunk);
	chunk = descriptorSet.Allocation(1, 0 + GetDescriptorBindingOffset(DESCRIPTOR_TYPE::TEXTURE_SHADER_RESOURCE_VIEW));
    textureView.Create(&device, texture, sampler, chunk);
	chunk = descriptorSet.Allocation(1, 0 + GetDescriptorBindingOffset(DESCRIPTOR_TYPE::SAMPLER));
	samplerView.Create(&device, sampler, chunk);

    descriptorSet.Update();

    // Make Draw Command
    auto& commandLists = device.GetCommandLists();
    for(uint32 i = 0; i < commandLists.size(); ++i){
        auto& commandList = commandLists[i];
        commandList.Open();
        auto& frameBuffer = swapChain.GetFrameBuffer();
        renderPass.Begin(commandList, frameBuffer.GetFrameBuffer(i), clearValue, swapChain.GetDisplaySize());
        pipelineState.Apply(commandList);
        descriptorSet.Apply(commandList, pipelineState.GetPipelineLayout());

		meshRenderer.Draw(commandList);

        renderPass.End(commandList);
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

	revMatrix44 t;
	t.RotationXYZ(revVector3(MathUtil::ToRadian(1.0f), MathUtil::ToRadian(2.0f), MathUtil::ToRadian(4.0f)));
	cbufferData.world *= t;
	cbufferData.wvp = cbufferData.world * cbufferData.view * cbufferData.projection;
	cbufferData.wvp.Transpose();
	constantBuffer->Update(&cbufferData, sizeof(cbufferData));


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
