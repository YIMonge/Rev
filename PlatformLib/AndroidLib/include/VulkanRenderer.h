#ifndef __VULKANRENDERER_H__
#define __VULKANRENDERER_H__

#include "IRenderer.h"
#ifdef _USE_VULKAN
#include "VulkanDeviceContext.h"
#include "VulkanSwapChain.h"
#include "VulkanRenderInfo.h"
#include "VulkanFrameBuffer.h"

class VulkanRenderer : public IRenderer
{
public:
    VulkanRenderer();
    virtual ~VulkanRenderer();

	virtual void StartUp(Window* window, const GraphicsDesc& desc);
	virtual void ShutDown();

	virtual void RenderBegin();
	virtual void RenderEnd();

	virtual void Clear(bool clear_color, bool clear_depth, const revColor& fill_color);
	virtual void SwapBuffers();

	virtual void SetBlendFunc(BLEND_FUNC func);
	virtual void SetAlphaTest(ALPHA_TEST func, float value);

private:
	bool CreateCommandPool();
	void DestroyCommandPool();
    void setImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkPipelineStageFlags srcStages, VkPipelineStageFlags destStages);

	VulkanDeviceContext context;
	VulkanSwapChain swapChain;
	VulkanRenderInfo renderInfo;
	VulkanFrameBuffer frameBuffer;

	VkQueue queue;
	VkCommandPool commandPool;
	revArray<VkCommandBuffer> commandBuffers;

	// clear color
	VkClearValue clearValue;
};

#endif
#endif
