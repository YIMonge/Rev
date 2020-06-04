#ifndef __VULKANRENDERER_H__
#define __VULKANRENDERER_H__
#ifdef _USE_VULKAN

#include "revRenderer.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanFrameBuffer.h"
#include "VulkanBuffer.h"
#include "VulkanTextureView.h"
#include "VulkanSampler.h"
#include "VulkanDescriptorSetLayout.h"
#include "VulkanDescriptorSet.h"
#include "VulkanPipelineState.h"
#include "VulkanRenderPass.h"

class VulkanRenderer : public revRenderer
{
public:
    VulkanRenderer();
    virtual ~VulkanRenderer();

	virtual void StartUp(Window* window, const GraphicsDesc& desc);
	virtual void ShutDown();

	void Render();
	void SwapBuffers();
private:
    void ExecuteCommand(revArray<revGraphicsCommandList>& lists, bool needSemaphore = false);
    void ExecuteCommand(revGraphicsCommandList& list, bool needSemaphore = false);

    void setImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkPipelineStageFlags srcStages, VkPipelineStageFlags destStages);
	VulkanDevice device;
	VulkanSwapChain swapChain;
	VulkanFrameBuffer frameBuffer;
	VulkanDescriptorSet descriptorSet;
	VulkanDescriptorSetLayout descriptorSetLayout;
	VulkanPipelineState pipelineState;

	// clear color
	revColor clearValue;

	// TEST CODE(resource)
	VulkanVertexBuffer triangleVertexBuffer;
	revVector4 cbufferOffset;
	VulkanConstantBuffer constantBuffer;
	VulkanTexture texture;
	VulkanTextureView textureView;
	VulkanSampler sampler;
	VulkanRenderPass renderPass;
	revMaterial mat;
};

#endif
#endif
