#ifndef __DX12SWAPCHAIN_H__
#define __DX12SWAPCHAIN_H__

#if defined(_USE_DIRECTX12)
#include "DX12DeviceContext.h"
#include "Window.h"
#include "revArray.h"

class DX12SwapChain
{
public:
	DX12SwapChain();
	~DX12SwapChain();

	bool Create(const DX12DeviceContext& deviceContext, const Window& window);
	void Destroy();

	int GetCurrentFrameIndex() const
	{
		return frameIndex;
	}


private:
	IDXGISwapChain3* swapChain;
	int frameIndex;

	ID3D12DescriptorHeap* renderTargetViewHeap;
	ID3D12DescriptorHeap* depthStencilViewHeap;
	revArray<ID3D12Resource*> renderTarget;
	ID3D12Resource* depthStencil;
};

#endif
#endif
