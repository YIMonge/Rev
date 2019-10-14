#ifndef __GLDEVICECONTEXT_H__
#define __GLDEVICECONTEXT_H__

#include "../../interface/IDeviceContext.h"

#if defined(_USE_VULKAN)
#include <windows.h>
#include "../include/libs/GL/glew.h"
#include "../include/libs/GL/wglew.h"
#include <GL/gl.h>
#include <GL/glu.h>

#pragma comment(lib, "OpenGL32.Lib")
#pragma comment(lib, "glu32.lib")


class VKDeviceContext : public IDeviceContext
{
public:
	VKDeviceContext();
	virtual ~VKDeviceContext();

	void Create(HWND hwnd);
	void Destroy();

	HDC GetDC();
	HGLRC GetRC();

	void BeginGL();
	void EndGL();

private:
	HWND hwnd;
	HDC hdc;
	HGLRC hrc;
};


inline HDC VKDeviceContext::GetDC()
{
	if (hdc == nullptr) {
		hdc = ::GetDC(hwnd);
	}
	return hdc;
}

inline HGLRC VKDeviceContext::GetRC()
{
	if (hrc == nullptr) {
		hrc = wglCreateContext(hdc);
	}
	return hrc;
}

#endif

#endif