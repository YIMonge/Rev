#ifndef __DX11RENDERER_H__
#define __DX11RENDERER_H__

#include "revRenderer.h"
#include "DX11DeviceContext.h"

#if defined(_USE_DIRECTX11)

class Window;

class DX11Renderer : public revRenderer
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
	DX11DeviceContext device_context;
};

#endif

#endif