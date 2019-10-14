#include "../include/VKDeviceContext.h"

#if defined(_USE_VULKAN)


VKDeviceContext::VKDeviceContext() :
	hdc(nullptr),
	hrc(nullptr)
{
}

/* virtual */ VKDeviceContext::~VKDeviceContext()
{
	Destroy();
}

void VKDeviceContext::Create(HWND _hwnd)
{
	hwnd = _hwnd;
}

void VKDeviceContext::Destroy()
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

void VKDeviceContext::BeginGL()
{
	hdc = ::GetDC(hwnd);
	wglMakeCurrent(hdc, hrc);
}

void VKDeviceContext::EndGL()
{
	::wglMakeCurrent(NULL, NULL);
	::ReleaseDC(hwnd, hdc);
	hdc = nullptr;
}

#endif
