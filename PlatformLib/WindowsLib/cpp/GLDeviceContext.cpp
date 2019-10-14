#include "../include/GLDeviceContext.h"

#if defined(_USE_OPENGL)


GLDeviceContext::GLDeviceContext() :
	hdc(nullptr),
	hrc(nullptr)
{
}

/* virtual */ GLDeviceContext::~GLDeviceContext()
{
	Destroy();
}

void GLDeviceContext::Create(HWND _hwnd)
{
	hwnd = _hwnd;
}

void GLDeviceContext::Destroy()
{
	if (hrc != nullptr)
	{
		wglDeleteContext(hrc);
		hrc = nullptr;
	}
	if (hdc != nullptr){
		ReleaseDC(hwnd, hdc);
		hdc = nullptr;
	}
}

void GLDeviceContext::BeginGL()
{
	hdc = ::GetDC(hwnd);
	wglMakeCurrent(hdc, hrc);
}

void GLDeviceContext::EndGL()
{
	::wglMakeCurrent(NULL, NULL);
	::ReleaseDC(hwnd, hdc);
	hdc = nullptr;
}

#endif
