#ifndef __GLESRENDERER_H__
#define __GLESRENDERER_H__

#include "GLESDeviceContext.h"
#include "../../interface/IRenderer.h"
#include "../include/Window.h"

#ifdef _USE_GLES

#include <EGL/egl.h>
#include <GLES/gl.h>

class GLESGraphics : public IRenderer
{
public:
	virtual void StartUp(Window* window, const GraphicsDesc& desc);
	virtual void ShutDown();
	virtual void SwapBuffers();

private:
	/*
	Window* main_window;
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	int width;
	int height;
	*/
};

#endif

#endif
