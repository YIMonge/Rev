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

	void Render();

	void Clear(bool clear_color, bool clear_depth, const revColor& fill_color);
	void SwapBuffers();

	void SetBlendFunc(BLEND_FUNC func);
	void SetAlphaTest(ALPHA_TEST func, float value);
private:
	bool CreateCommandPool();
	void DestroyCommandPool();
    void setImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkPipelineStageFlags srcStages, VkPipelineStageFlags destStages);


	VulkanDeviceContext context;
	VulkanSwapChain swapChain;
	VulkanRenderInfo renderInfo;
	VulkanFrameBuffer frameBuffer;

	VkCommandPool commandPool;
	revArray<VkCommandBuffer> commandBuffers;

	// clear color
	VkClearValue clearValue;

	// Draw Command

};

#endif
#endif
