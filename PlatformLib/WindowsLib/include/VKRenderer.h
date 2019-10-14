#ifndef __GLRENDERER_H__
#define __GLRENDERER_H__

#include "VKDeviceContext.h"
#include "../../interface/IRenderer.h"

#if defined(_USE_VULKAN)

class Window;

class VkRenderer : public IRenderer
{
public:
	virtual void StartUp(Window* window, const GraphicsDesc& desc);
	virtual void ShutDown();

	virtual void RenderBegin();
	virtual void RenderEnd();

	virtual void Clear(bool clear_color, bool clear_depth, const revColor& fill_color);
	virtual void SwapBuffers();

	virtual void SetBlendFunc(BLEND_FUNC func) {}
	virtual void SetAlphaTest(ALPHA_TEST func, float value) {}

private:
	Window* main_window;
	VKDeviceContext device;
};

#endif

#endif
