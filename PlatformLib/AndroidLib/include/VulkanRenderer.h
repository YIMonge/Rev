#ifndef __VULKANRENDERER_H__
#define __VULKANRENDERER_H__

#include "revRenderer.h"
#ifdef _USE_VULKAN
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanRenderInfo.h"
#include "VulkanFrameBuffer.h"
#include "VulkanBuffer.h"

class VulkanRenderer : public revRenderer
{
public:
    VulkanRenderer();
    virtual ~VulkanRenderer();

	virtual void StartUp(Window* window, const GraphicsDesc& desc);
	virtual void ShutDown();

	void Render();

	void Clear(bool clear_color, bool clear_depth, const revColor& fill_color);
	void SwapBuffers();

	void SetBlendFunc(BLEND_FUNC func);
	void SetAlphaTest(ALPHA_TEST func, float value);
private:
	bool CreateCommandPool();
	void DestroyCommandPool();
    void setImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkPipelineStageFlags srcStages, VkPipelineStageFlags destStages);


	VulkanDevice device;
	VulkanSwapChain swapChain;
	VulkanRenderInfo renderInfo;
	VulkanFrameBuffer frameBuffer;

	VkCommandPool commandPool;
	revArray<revGraphicsCommandBuffer> commandBuffers;

	// clear color
	VkClearValue clearValue;

	// TEST CODE
	VulkanBuffer triangleVertexBuffer;
	VulkanBuffer triangleTexcoordBuffer;
};

#endif
#endif
