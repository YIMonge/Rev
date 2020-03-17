#ifndef __GLRENDERER_H__
#define __GLRENDERER_H__

#include "GLDeviceContext.h"
#include "revRenderer.h"

#if defined(_USE_OPENGL)

class Window;

class GLRenderer : public revRenderer
{
public:
	virtual void StartUp(Window* window, const GraphicsDesc& desc);
	virtual void ShutDown();

	virtual void RenderBegin();
	virtual void RenderEnd();

	virtual void Clear(bool clear_color, bool clear_depth, const revColor& fill_color);
	virtual void SwapBuffers();

private:
	Window* main_window;
	GLDeviceContext device;
};

#endif

#endif
