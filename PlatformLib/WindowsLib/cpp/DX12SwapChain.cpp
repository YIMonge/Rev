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
	if (FAILED(hr)) {
		if (dxgiFactory != nullptr) dxgiFactory->Release();
		return false;
	}

	GraphicsDesc graphicsDesc  = deviceContext.GetDesc();
	uint32 bufferNum = graphicsDesc.GetBufferNum();

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
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Windowed = !graphicsDesc.useFullscreen;

	IDXGISwapChain* tempSwapChain;
	hr = dxgiFactory->CreateSwapChain(
		deviceContext.GetCommandQueue(),
		&swapChainDesc,
		&tempSwapChain);
	if (FAILED(hr)) {
		if (dxgiFactory != nullptr) dxgiFactory->Release();
		if (tempSwapChain != nullptr) tempSwapChain->Release();
		return false;
	}
	// convert swapchian interface into IDXGISwapChain3 
	tempSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain));

	frameIndex = swapChain->GetCurrentBackBufferIndex();


	// create descriptorheap for render target 
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	memset(&heapDesc, 0, sizeof(heapDesc));
	heapDesc.NumDescriptors = bufferNum;
	heapDesc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	auto device = deviceContext.GetDevice();
	hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&renderTargetViewHeap));
	if (FAILED(hr)) {
		return false;
	}

	// create descriptorheap for depth stencil 
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&depthStencilViewHeap));
	if (FAILED(hr)) {
		return false;
	}

	// create render target view 
	auto handle = renderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_RENDER_TARGET_VIEW_DESC renderTargetDesc;
	renderTargetDesc.Format = formats[static_cast<int>(graphicsDesc.bufferFormat)];
	renderTargetDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	renderTargetDesc.Texture2D.MipSlice = 0;
	renderTargetDesc.Texture2D.PlaneSlice = 0;

	renderTarget.Resize(bufferNum);
	for (uint32 i = 0; i < bufferNum; ++i) {
		hr = swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTarget[i]));
		if (FAILED(hr)) {
			return false;
		}
		device->CreateRenderTargetView(renderTarget[i], &renderTargetDesc, handle);
		handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	}

	// create depth stencil view 
	D3D12_HEAP_PROPERTIES heapProp;
	memset(&heapProp, 0, sizeof(heapProp));
	heapProp.Type					= D3D12_HEAP_TYPE_DEFAULT;
	heapProp.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask		= 1;
	heapProp.VisibleNodeMask		= 1;

	D3D12_RESOURCE_DESC resourceDesc;
	memset(&resourceDesc, 0, sizeof(resourceDesc));
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = window.GetWidth();
	resourceDesc.Height = window.GetHeight();
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 0;
	resourceDesc.Format = DXGI_FORMAT_D32_FLOAT;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	// set clear value for depth stencil 
	D3D12_CLEAR_VALUE clearValue;
	memset(&clearValue, 0, sizeof(clearValue));
	clearValue.Format = DXGI_FORMAT_D32_FLOAT;
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;

	hr = device->CreateCommittedResource(&heapProp, 
		D3D12_HEAP_FLAG_NONE, 
		&resourceDesc, 
		D3D12_RESOURCE_STATE_DEPTH_WRITE, 
		&clearValue,
		IID_PPV_ARGS(&depthStencil));
	if (FAILED(hr)) {
		return false;
	}

	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc;
	memset(&depthStencilDesc, 0, sizeof(depthStencilDesc));
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

	device->CreateDepthStencilView(depthStencil, &depthStencilDesc, depthStencilViewHeap->GetCPUDescriptorHandleForHeapStart());

	dxgiFactory->Release();
	return true;
}

void DX12SwapChain::Destroy()
{
	for (uint32 i = 0; i < renderTarget.Count(); ++i) {
		if (renderTarget[i] != nullptr) {
			renderTarget[i]->Release();
			renderTarget[i] = nullptr;
		}
	}

	if (swapChain != nullptr) {
		swapChain->Release();
		swapChain = nullptr;
	}
}

#endif