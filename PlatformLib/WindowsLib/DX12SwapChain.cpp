#include "DX12SwapChain.h"
#if defined(_USE_DIRECTX12)

DX12SwapChain::DX12SwapChain() :
swapChain(nullptr)
{
}

DX12SwapChain::~DX12SwapChain()
{
}

bool DX12SwapChain::Create(const DX12DeviceContext& deviceContext, const Window& window)
{
	IDXGIFactory4* dxgiFactory;
	HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	if (hr != S_OK) {
		if (dxgiFactory != nullptr) dxgiFactory->Release();
		return false;
	}

	GraphicsDesc graphicsDesc  = deviceContext.GetDesc();

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	memset(&swapChainDesc, 0, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = graphicsDesc.GetBufferNum();

	DXGI_FORMAT formats[] = 
	{
		DXGI_FORMAT_B8G8R8A8_UNORM,		// R8B8G8A8
	};
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.Format = formats[static_cast<int>(graphicsDesc.bufferFormat)];
	swapChainDesc.BufferDesc.Width = window.GetWidth();
	swapChainDesc.BufferDesc.Height = window.GetHeight();
	swapChainDesc.OutputWindow = window.GetHWnd();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Windowed = !graphicsDesc.useFullscreen;

	hr = dxgiFactory->CreateSwapChain(
		deviceContext.GetCommandQueue(),
		&swapChainDesc,
		&swapChain);
	if (hr != S_OK) {
		if (dxgiFactory != nullptr) dxgiFactory->Release();
		if (swapChain != nullptr) swapChain->Release();
		swapChain = nullptr;
		return false;
	}

	dxgiFactory->Release();
	return true;
}

void DX12SwapChain::Destroy()
{
	if (swapChain != nullptr) {
		swapChain->Release();
		swapChain = nullptr;
	}
}

#endif
