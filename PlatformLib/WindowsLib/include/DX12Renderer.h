#ifndef __DX12RENDERER_H__
#define __DX12RENDERER_H__

#include "../../interface/IRenderer.h"
#include "DX12DeviceContext.h"

#if defined(_USE_DIRECTX12)

class Window;

class DX12Renderer : public IRenderer
{
public:
	virtual void StartUp(Window* window, const GraphicsDesc& desc);
	virtual void ShutDown();

	virtual void RenderBegin() {}
	virtual void RenderEnd() {}

	virtual void Clear(bool clear_color, bool clear_depth, const revColor& fill_color) {}
	virtual void SwapBuffers() {}

	virtual void SetBlendFunc(BLEND_FUNC func) {}
	virtual void SetAlphaTest(ALPHA_TEST func, float value) {}

private:
	Window* main_window;
	DX12DeviceContext device_context;
};

#endif

#endif
