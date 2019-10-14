#include "../include/DX12Renderer.h"
#include "../include/Window.h"


#if defined(_USE_DIRECTX12)

void DX12Renderer::StartUp(Window* window, const GraphicsDesc& desc)
{
	bool is_created = device_context.Create(window, desc);
	if( !is_created ){
		// TODO:Shutdown Application
	}



}

void DX12Renderer::ShutDown()
{
}

#endif