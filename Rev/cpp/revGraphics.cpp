#include "revGraphics.h"

#ifdef _WINDOWS
	#if	 defined(_USE_OPENGL)
	#include "GLRenderer.h"
	#elif defined(_USE_DIRECTX11)
	#include "DX11Renderer.h"
	#elif defined(_USE_DIRECTX12)
	#include "DX12Renderer.h"
	#elif defined(_USE_VULKAN)
	#include "VkRenderer.h"
	#endif
#elif defined(_ANDROID)
	#if defined(_USE_GLES)
	#include "GLESRenderer.h"
	#elif defined(_USE_VULKAN)
	#include "VkRenderer.h"
	#endif
#endif

#include "Window.h"

void revGraphics::StartUp(Window* window, const GraphicsDesc& desc)
{
	this->desc = desc;

#if defined(_USE_OPENGL)
	renderer = new GLRenderer();
#elif defined(_USE_DIRECTX11)
	renderer = new DX11Renderer();
#elif defined(_USE_DIRECTX12)
	renderer = new DX12Renderer();
#elif defined(_USE_VULKAN)
	renderer = new VkRenderer();
#else
	renderer = new GLESRenderer();
#endif
	renderer->StartUp(window, desc);
}

void revGraphics::ShutDown()
{
	renderer->ShutDown();
	delete renderer;
	renderer = nullptr;
}

void revGraphics::RenderBegin()
{
	renderer->RenderBegin();
}

void revGraphics::RenderEnd()
{
	renderer->RenderEnd();
}

void revGraphics::Clear(bool clear_color, bool clear_depth, const revColor& fill_color)
{
	renderer->Clear(clear_color, clear_depth, fill_color);
}

void revGraphics::SwapBuffers()
{
	renderer->SwapBuffers();
}
