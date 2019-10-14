#include "../include/DX11Renderer.h"
#include "../include/Window.h"
#include "revColor.h"

#if defined(_USE_DIRECTX11)

ID3D11RenderTargetView* RenderTargetView = NULL;

void DX11Renderer::StartUp(Window* window, const GraphicsDesc& desc)
{
	bool is_created = device_context.Create(window, desc);
	if( !is_created ){
		// TODO:Shutdown Application
	}

	// create render target view
	ID3D11Texture2D *BackBuffer;
	device_context.GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBuffer);
	device_context.GetDevice()->CreateRenderTargetView(BackBuffer, NULL, &RenderTargetView);
	BackBuffer->Release();
	device_context.GetDeviceContext()->OMSetRenderTargets(1, &RenderTargetView, NULL);

	window->Disp();
}

void DX11Renderer::ShutDown()
{
}

void DX11Renderer::RenderBegin()
{

}

void DX11Renderer::RenderEnd()
{

}

void DX11Renderer::Clear(bool clear_color, bool clear_depth, const revColor& fill_color)
{
	device_context.GetDeviceContext()->ClearRenderTargetView(RenderTargetView, fill_color.data);
}

void DX11Renderer::SwapBuffers()
{
	device_context.GetSwapChain()->Present(0, 0);
}

#endif