#include "revGraphics.h"

#ifdef _WINDOWS
	#if	 defined(_USE_OPENGL)
	#include "GLRenderer.h"
	#elif defined(_USE_DIRECTX11)
	#include "DX11Renderer.h"
	#elif defined(_USE_DIRECTX12)
	#include "DX12Renderer.h"
	#endif
#elif defined(_ANDROID)
	#if defined(_USE_GLES)
	#include "GLESRenderer.h"
	#elif defined(_USE_VULKAN)
	#include "VulkanRenderer.h"
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
	renderer = new VulkanRenderer();
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

void revGraphics::Draw()
{
	// TEST for draw command storage
	revDrawCommand* a[512];
	int n = rand() % 512;
	for(int i = 0; i < n; ++i) {
		a[i] = drawCommandStorage.Borrow();
	}
	for(int i = 0; i < n; ++i){
		drawCommandStorage.Return(a[i]);
	}
	renderer->Render();
}