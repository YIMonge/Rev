#ifndef __DX11DEVICECONTEXT_H__
#define __DX11DEVICECONTEXT_H__

#include "../../interface/IDeviceContext.h"

#if defined(_USE_DIRECTX11)

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

class Window;

class DX11DeviceContext : public IDeviceContext
{
public:
	bool Create(Window* windw, const GraphicsDesc& desc);

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	IDXGISwapChain* GetSwapChain();
private:
	ID3D11Device* device;
	ID3D11DeviceContext* device_context;
	IDXGISwapChain* swap_chain;
};

inline ID3D11Device* DX11DeviceContext::GetDevice()
{
	return device;
}
	
inline ID3D11DeviceContext* DX11DeviceContext::GetDeviceContext()
{
	return device_context;
}
	
inline IDXGISwapChain* DX11DeviceContext::GetSwapChain()
{
	return swap_chain; 
}

#endif

#endif