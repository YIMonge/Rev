#include "../include/GLRenderer.h"
#include "../include/Window.h"
#include "revColor.h"

#if defined(_USE_OPENGL)

#include <assert.h>

void GLRenderer::StartUp(Window* window, const GraphicsDesc& desc)
{
	if (window == nullptr){
		main_window = new Window();
		if (main_window == nullptr){
			return;
		}
		main_window->Create("MainWindow", 1280, 720, 0, 0);
	}
	else{
		main_window = window;
	}

	device.Create(main_window->GetHWnd());

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize			= sizeof(pfd);
	pfd.nVersion		= 1;
	pfd.dwFlags			= (PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER);
	pfd.iPixelType		= PFD_TYPE_RGBA;
	pfd.cColorBits		= 32;
	pfd.cRedBits		= 8;
	pfd.cColorBits		= 32;
	pfd.cRedBits		= 8;
	pfd.cRedShift		= 0;
	pfd.cGreenBits		= 8;
	pfd.cGreenShift		= 0;
	pfd.cBlueBits		= 8;
	pfd.cBlueShift		= 0;
	pfd.cAlphaBits		= 8;
	pfd.cAlphaShift		= 0;
	pfd.cAccumBits		= 0;
	pfd.cAccumRedBits	= 0;
	pfd.cAccumGreenBits = 0;
	pfd.cAccumBlueBits	= 0;
	pfd.cAccumAlphaBits = 0;
	pfd.cDepthBits		= 32;
	pfd.cStencilBits	= 0;
	pfd.cAuxBuffers		= 0;
	pfd.iLayerType		= PFD_MAIN_PLANE;
	pfd.bReserved		= 0;
	pfd.dwLayerMask		= 0;
	pfd.dwVisibleMask	= 0;
	pfd.dwDamageMask	= 0;


	HDC hdc = device.GetDC();
	int pf = ChoosePixelFormat(hdc , &pfd);
	if (pf == 0){
		return;
	}

	if (!SetPixelFormat(hdc, pf, &pfd)) {
		return;
	}

	::DescribePixelFormat(hdc, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	HGLRC hrc = device.GetRC();
	wglMakeCurrent(hdc, hrc);

	GLenum  glewError = glewInit();
	if (glewError != GLEW_OK) {
		assert(false);
		return;
	}

	main_window->Disp();

	wglSwapIntervalEXT(1);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	SwapBuffers();

	wglMakeCurrent(NULL, NULL);

}

void GLRenderer::ShutDown()
{
	device.Destroy();
	if (main_window != nullptr){		
		main_window->Hide();
		main_window->Destroy();
		delete main_window;
		main_window = nullptr;
	}
}

void GLRenderer::RenderBegin()
{
	device.BeginGL();
}

void GLRenderer::RenderEnd()
{
	device.EndGL();
}

void GLRenderer::Clear(bool clear_color, bool clear_depth, const revColor& fill_color)
{
	GLuint flag = 0;
	if (clear_color)
	{
		flag |= GL_COLOR_BUFFER_BIT;
		glClearColor(fill_color.r, fill_color.g, fill_color.b, fill_color.a);
	}
	if (clear_depth)
	{
		flag |= GL_DEPTH_BUFFER_BIT;
	}
	glClear(flag);
}

void GLRenderer::SwapBuffers()
{
	glFlush();
	::SwapBuffers(device.GetDC());
}

#endif