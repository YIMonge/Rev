#include "../include/GLESDeviceContext.h"

#ifdef _USE_GLES

#include <EGL/egl.h>
#include <GLES/gl.h>

GLESDeviceContext::GLESDeviceContext()
{
}

/* virtual */ GLESDeviceContext::~GLESDeviceContext()
{
	Destroy();
}

void GLESDeviceContext::Create()
{
}

void GLESDeviceContext::Destroy()
{
}

#endif