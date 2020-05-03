#ifndef __VULKANRENDERER_H__
#define __VULKANRENDERER_H__

#include "revRenderer.h"
#ifdef _USE_VULKAN
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanRenderInfo.h"
#include "VulkanFrameBuffer.h"
#include "VulkanBuffer.h"
#include "VulkanTextureView.h"
#include "VulkanSampler.h"
#include "VulkanDescriptorSetLayout.h"
#include "VulkanDescriptorSet.h"

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
    void ExecuteCommand(revArray<revGraphicsCommandList>& lists);
    void ExecuteCommand(revGraphicsCommandList& list);

    void setImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkPipelineStageFlags srcStages, VkPipelineStageFlags destStages);
	VulkanDevice device;
	VulkanSwapChain swapChain;
	//VulkanRenderInfo renderInfo;
	VulkanFrameBuffer frameBuffer;

	// clear color
	VkClearValue clearValue;

	// TEST CODE
	VulkanBuffer triangleVertexBuffer;
	VulkanTexture texture;
	VulkanTextureView textureView;
	VulkanSampler sampler;
	VulkanDescriptorSet descriptorSet[4];
	VulkanDescriptorSetLayout descriptorSetLayout;
	revMaterial mat;
};

#endif
#endif
