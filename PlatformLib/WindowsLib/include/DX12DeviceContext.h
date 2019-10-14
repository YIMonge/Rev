#ifndef __DX12DEVICECONTEXT_H__
#define __DX12DEVICECONTEXT_H__

#include "../../interface/IDeviceContext.h"

#if defined(_USE_DIRECTX12)

#include <d3d12.h>
#include <dxgi1_4.h>
#pragma comment(lib, "d3d12.lib")

class Window;

class DX12DeviceContext : public IDeviceContext
{
public:
	bool Create(Window* windw, const GraphicsDesc& desc);

	ID3D12Device* GetDevice() const;
	IDXGISwapChain* GetSwapChain() const;
private:
	ID3D12Device*				device;
	IDXGISwapChain*				swap_chain;
	IDXGIFactory1*				dxgi_factroy;
	ID3D12CommandQueue*			command_queue;
	ID3D12CommandAllocator*	command_allocator;
	ID3D12Resource*				render_targets;

};

inline ID3D12Device* DX12DeviceContext::GetDevice() const
{
	return device;
}
	
inline IDXGISwapChain* DX12DeviceContext::GetSwapChain() const
{
	return swap_chain; 
}


#endif

#endif


