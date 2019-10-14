#ifndef __VkRenderer_H__
#define __VkRenderer_H__

#include "IRenderer.h"
#ifdef _USE_VULKAN
#include "VkDeviceContext.h"


class VkRenderer : public IRenderer
{
public:
    VkRenderer();
    virtual ~VkRenderer();

	virtual void StartUp(Window* window, const GraphicsDesc& desc);
	virtual void ShutDown();

	virtual void RenderBegin();
	virtual void RenderEnd();

	virtual void Clear(bool clear_color, bool clear_depth, const revColor& fill_color);
	virtual void SwapBuffers();

	virtual void SetBlendFunc(BLEND_FUNC func);
	virtual void SetAlphaTest(ALPHA_TEST func, float value);

private:
	VkDeviceContext context;
};

#endif
#endif
