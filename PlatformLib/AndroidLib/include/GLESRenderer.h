#ifndef __GLESRENDERER_H__
#define __GLESRENDERER_H__

#include "GLESDeviceContext.h"
#include "../../interface/IRenderer.h"
#include "../include/Window.h"

#ifdef _USE_GLES

#include <EGL/egl.h>
#include <GLES/gl.h>

class GLESRenderer : public IRenderer
{
public:
    GLESRenderer();
	virtual ~GLESRenderer(){};

	virtual void StartUp(Window* window, const GraphicsDesc& desc);
	virtual void ShutDown();

	virtual void RenderBegin();
	virtual void RenderEnd();
	virtual void Clear(bool clear_color, bool clear_depth, const revColor& fill_color);	
	virtual void SwapBuffers();

	virtual void SetBlendFunc(BLEND_FUNC func);
	virtual void SetAlphaTest(ALPHA_TEST func, float value);	

private:
	Window* main_window;
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	int width;
	int height;
};

#endif
#endif
