#ifdef _USE_DIRECTX12

#include "DX12SwapChain.h"
#include "Log.h"

DX12SwapChain::DX12SwapChain() :
swapChain(nullptr)
{
}

DX12SwapChain::~DX12SwapChain()
{
}

bool DX12SwapChain::Create(DX12Device* device, const Window& window)
{
	this->device = device;

	uint32 dxgiFlags = 0;
#ifdef _DEBUG
	dxgiFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	IDXGIFactory4* dxgiFactory;
	HRESULT hr = CreateDXGIFactory2(dxgiFlags, IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(hr)) {
		if (dxgiFactory != nullptr) dxgiFactory->Release();
		NATIVE_LOGE("failed to create dxgi factory");
		return false;
	}

	GraphicsDesc graphicsDesc  = device->GetDesc();
	uint32 bufferNum = graphicsDesc.GetBufferNum();

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	memset(&swapChainDesc, 0, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = graphicsDesc.GetBufferNum();

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.Format = ConvertToDXFormat(GRAPHICS_FORMAT::R8G8B8A8_UNORM); // TODO: 
	swapChainDesc.BufferDesc.Width = window.GetWidth();
	swapChainDesc.BufferDesc.Height = window.GetHeight();
	swapChainDesc.OutputWindow = window.GetHWnd();
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Windowed = !graphicsDesc.useFullscreen;

	IDXGISwapChain* tempSwapChain;
	hr = dxgiFactory->CreateSwapChain(
		device->GetQueue(),
		&swapChainDesc,
		&tempSwapChain);
	if (FAILED(hr)) {
		if (dxgiFactory != nullptr) dxgiFactory->Release();
		if (tempSwapChain != nullptr) tempSwapChain->Release();
		NATIVE_LOGE("failed to create swapchain");
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

	auto dxdevice = device->GetDevice();
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetDesc;
	renderTargetDesc.Format = ConvertToDXFormat(GRAPHICS_FORMAT::R8G8B8A8_UNORM); // TODO: 
	renderTargetDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	renderTargetDesc.Texture2D.MipSlice = 0;
	renderTargetDesc.Texture2D.PlaneSlice = 0;

	renderTargetHeap.Create(device, DESCRIPTOR_HEAP_TYPE::RENDER_TARGET, bufferNum, false);
	depthStencilHeap.Create(device, DESCRIPTOR_HEAP_TYPE::DEPTH_STENCIL, bufferNum, false);

	renderTarget.resize(bufferNum);
	auto chunkForRenderTarget = renderTargetHeap.Allocation(bufferNum);
	for (uint32 i = 0; i < bufferNum; ++i) {
		hr = swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTarget[i]));
		if (FAILED(hr)) {
			NATIVE_LOGE("failed to get render target");
			return false;
		}
		dxdevice->CreateRenderTargetView(renderTarget[i], &renderTargetDesc, chunkForRenderTarget.GetHandle(i));
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

	hr = dxdevice->CreateCommittedResource(&heapProp,
		D3D12_HEAP_FLAG_NONE, 
		&resourceDesc, 
		D3D12_RESOURCE_STATE_DEPTH_WRITE, 
		&clearValue,
		IID_PPV_ARGS(&depthStencil));
	if (FAILED(hr)) {
		NATIVE_LOGE("failed to create depth/stencil");
		return false;
	}

	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc;
	memset(&depthStencilDesc, 0, sizeof(depthStencilDesc));
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

	auto chunkForDepthStencil = depthStencilHeap.Allocation(bufferNum);
	dxdevice->CreateDepthStencilView(depthStencil, &depthStencilDesc, chunkForDepthStencil.GetHandle());

	dxgiFactory->Release();

	// create fence 
	hr = dxdevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	if (FAILED(hr)) {
		NATIVE_LOGE("failed to create fence");
		return false;
	}
	fenceValue = 1;
	fenceEvent = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
	if (fenceEvent == nullptr) {
		NATIVE_LOGE("failed to create event for sync");
		return false;
	}

	return true;
}

void DX12SwapChain::Appply(DX12CommandList& commandList, const revColor& clearColor)
{
	auto dxCommandList = commandList.GetList();
	ID3D12Resource* renderTarget = GetCurrentRenderTarget();

	D3D12_RESOURCE_BARRIER barrier;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.pResource = renderTarget;

	auto renderTargetHandle = renderTargetHeap.GetCPUHandle(frameIndex);
	auto depthStenclisHandle = depthStencilHeap.GetCPUHandle();
	dxCommandList->ResourceBarrier(1, &barrier);
	dxCommandList->OMSetRenderTargets(1,
		&renderTargetHandle,
		FALSE,
		//nullptr);
		&depthStenclisHandle);
	dxCommandList->ClearRenderTargetView(renderTargetHandle, clearColor.data, 0, nullptr);
	dxCommandList->ClearDepthStencilView(depthStenclisHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// for close
	commandList.AddTransitionBarrier(renderTarget,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT
		);
}


bool DX12SwapChain::WaitForPreviousFrame()
{
    ID3D12CommandQueue* queue = device->GetQueue();
	const uint64 tempFenceValue = fenceValue;
	HRESULT hr = queue->Signal(fence, tempFenceValue);
	if (FAILED(hr)) {
		NATIVE_LOGE("failed to set fence to command queue");
		return false;
	}
	fenceValue++;
	if (fence->GetCompletedValue() < tempFenceValue) {
		hr = fence->SetEventOnCompletion(tempFenceValue, fenceEvent);
		if (FAILED(hr)) {
			NATIVE_LOGE("failed to set event to fence");
			return false;
		}
		WaitForSingleObject(fenceEvent, INFINITE);
	}
	frameIndex = swapChain->GetCurrentBackBufferIndex();
	return true;
}


bool DX12SwapChain::Present() const
{
	HRESULT hr = swapChain->Present(1, 0);
	return (((HRESULT)(hr)) < 0);
}

void DX12SwapChain::Destroy()
{
	for (uint32 i = 0; i < renderTarget.size(); ++i) {
		if (renderTarget[i] != nullptr) {
			renderTarget[i]->Release();
			renderTarget[i] = nullptr;
		}
	}

	renderTargetHeap.Destroy();
	depthStencilHeap.Destroy();

	if (swapChain != nullptr) {
		swapChain->Release();
		swapChain = nullptr;
	}
}

#endif
