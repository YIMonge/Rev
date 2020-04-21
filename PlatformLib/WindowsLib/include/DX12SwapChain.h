#ifndef __DX12SWAPCHAIN_H__
#define __DX12SWAPCHAIN_H__

#ifdef _USE_DIRECTX12
#include "DX12Device.h"
#include "Window.h"
#include "revArray.h"
#include "revColor.h"

class DX12SwapChain
{
public:
	DX12SwapChain();
	~DX12SwapChain();

	bool Create(DX12Device* device, const Window& window);
	void Destroy();

	int32 GetCurrentFrameIndex() const
	{
		return frameIndex;
	}

	ID3D12Resource* GetCurrentRenderTarget() const
	{
		return renderTarget[frameIndex];
	}

	void Appply(DX12CommandList& commandList, const revColor& clearColor = revColor::white);

	bool Present() const;

	// TODO: swapchain should have fence, value, event. 
	bool WaitForPreviousFrame();

private:
	DX12Device* device;

	IDXGISwapChain3* swapChain;
	int32 frameIndex;

	ID3D12DescriptorHeap* renderTargetViewHeap;
	uint32 renderTargetViewDescriptorSize;
	ID3D12DescriptorHeap* depthStencilViewHeap;
	revArray<ID3D12Resource*> renderTarget;
	ID3D12Resource* depthStencil;

	ID3D12Fence* fence;
	uint32 fenceValue;
	HANDLE fenceEvent;
};

#endif
#endif
