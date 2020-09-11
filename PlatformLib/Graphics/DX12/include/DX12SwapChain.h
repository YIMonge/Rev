#ifndef __DX12SWAPCHAIN_H__
#define __DX12SWAPCHAIN_H__
#ifdef _USE_DIRECTX12

#include "DX12Device.h"
#include "DX12DescriptorHeap.h"
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

	void Appply(DX12CommandList& commandList, const revColor& clearColor = revColor::WHITE);
	bool Present();
private:
	DX12Device* device;

	IDXGISwapChain3* swapChain;
	int32 frameIndex;

	uint32 renderTargetViewDescriptorSize;
	revArray<ID3D12Resource*> renderTarget;
	ID3D12Resource* depthStencil;
	revDescriptorHeap* renderTargetHeap;
	revDescriptorHeap* depthStencilHeap;
	revDescriptorHeap::Chunk* renderTargetChunk;
	revDescriptorHeap::Chunk* depthStencilChunk;
};

#endif
#endif
