#ifndef __DX12RENDERER_H__
#define __DX12RENDERER_H__

#include "IRenderer.h"
#include "DX12DeviceContext.h"
#include "DX12SwapChain.h"
#include "revArray.h"

#if defined(_USE_DIRECTX12)

class Window;

class DX12Renderer : public IRenderer
{
public:
	virtual void StartUp(Window* window, const GraphicsDesc& desc);
	virtual void ShutDown();

	virtual void Render();

	virtual void Clear(bool clear_color, bool clear_depth, const revColor& fill_color) {}
	virtual void SwapBuffers() {}

	virtual void SetBlendFunc(BLEND_FUNC func) {}
	virtual void SetAlphaTest(ALPHA_TEST func, float value) {}

private:
	bool IntialzieForApp();

private:
	Window* main_window;
	DX12DeviceContext deviceContext;
	DX12SwapChain swapChain;

	ID3D12CommandAllocator*		commandAllocator;
	ID3D12GraphicsCommandList*	commandList;
	ID3D12CommandQueue*			commandQueue;

	D3D12_VIEWPORT viewport;
	D3D12_RECT rectScissor;

	// initalze for app 
	ID3D12RootSignature* rootSignature;
	ID3D12PipelineState* pipelineState;
};

#endif

#endif
