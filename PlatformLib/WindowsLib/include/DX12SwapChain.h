#ifndef __DX12SWAPCHAIN_H__
#define __DX12SWAPCHAIN_H__

#if defined(_USE_DIRECTX12)
#include "DX12DeviceContext.h"
#include "Window.h"

class DX12SwapChain
{
public:
	DX12SwapChain();
	~DX12SwapChain();

	bool Create(const DX12DeviceContext& deviceContext, const Window& window);
	void Destroy();

private:
	IDXGISwapChain* swapChain;

};

#endif
#endif
