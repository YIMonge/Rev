#include "GLESRenderer.h"
#include <android_native_app_glue.h>
#include <stdlib.h>
#include "revColor.h"

#ifdef _USE_GLES

GLESRenderer::GLESRenderer()
{}

void GLESRenderer::StartUp(Window* window, const GraphicsDesc& desc)
{
	main_window = window;

	const EGLint attribs[] = {
		EGL_SURFACE_TYPE,	EGL_WINDOW_BIT,
		EGL_BLUE_SIZE,		8,
		EGL_GREEN_SIZE,		8,
		EGL_RED_SIZE,		8,
		EGL_NONE
	};

	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(display, 0, 0);

	EGLConfig config;
	EGLint numConfigs;
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	EGLint format;
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(main_window->GetHandle(), 0, 0, format);
	surface = eglCreateWindowSurface(display, config, main_window->GetHandle(), NULL);
	context = eglCreateContext(display, config, NULL, NULL);

	eglMakeCurrent(display, surface, surface, context);

	eglQuerySurface(display, surface, EGL_WIDTH, &width);
    eglQuerySurface(display, surface, EGL_HEIGHT, &height);


    // Test Code
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);
	Clear(true, true, revColor(0.0f, 0.0f, 1.0f, 1.0f));
	SwapBuffers();
}

void GLESRenderer::ShutDown()
{
}

void GLESRenderer::RenderBegin()
{

}

void GLESRenderer::RenderEnd()
{

}

void GLESRenderer::Clear(bool clear_color, bool clear_depth, const revColor& fill_color)
{
	glClearColor(fill_color.r, fill_color.g, fill_color.b, fill_color.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GLESRenderer::SwapBuffers()
{
	eglSwapBuffers(display, surface);
}

void GLESRenderer::SetBlendFunc(BLEND_FUNC func)
{
}

void GLESRenderer::SetAlphaTest(ALPHA_TEST func, float value)
{
}

#endif