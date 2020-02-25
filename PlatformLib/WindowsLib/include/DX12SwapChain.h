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

	int32 GetCurrentFrameIndex() const
	{
		return frameIndex;
	}

	ID3D12Resource* GetCurrentRenderTarget() const
	{
		return renderTarget[frameIndex];
	}


	CD3DX12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle() const;

	bool Present() const;

	// TODO: swapchain should have fence, value, event. 
	bool WaitForPreviousFrame(ID3D12CommandQueue* queue, ID3D12Fence* fence, uint32& fenceValue, HANDLE fenceEvent);

private:
	IDXGISwapChain3* swapChain;
	int32 frameIndex;

	ID3D12DescriptorHeap* renderTargetViewHeap;
	uint32 renderTargetViewDescriptorSize;
	ID3D12DescriptorHeap* depthStencilViewHeap;
	revArray<ID3D12Resource*> renderTarget;
	ID3D12Resource* depthStencil;
};

#endif
#endif
