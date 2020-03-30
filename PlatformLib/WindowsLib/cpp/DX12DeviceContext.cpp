#include "../include/DX12DeviceContext.h"
#include "../include/Window.h"
#include "revTypedef.h"

#ifdef _USE_DIRECTX12

bool DX12DeviceContext::Create(const GraphicsDesc& desc)
{
#if defined(_DEBUG)
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->Release();
		debugController = nullptr;
	}
#endif

	this->desc = desc;

	// Create the Direct3D 12 API device object
	HRESULT hr = D3D12CreateDevice(
		nullptr,						// Specify nullptr to use the default adapter.
		D3D_FEATURE_LEVEL_11_0,			// Minimum feature level this app can support.
		IID_PPV_ARGS(&device)			// Returns the Direct3D device created.
		);
	if (FAILED(hr)) {
		return false;
	}

	// Create the command queue.
	D3D12_COMMAND_QUEUE_DESC queueDesc;
	memset(&queueDesc, 0, sizeof(queueDesc));

	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	hr = device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&queue));
	if (FAILED(hr)) {
		return false;
	}

	// create allocator and command list 
	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	if (FAILED(hr)) {
		return false;
	}
	return true;
}

bool DX12DeviceContext::CreateCommandList(ID3D12PipelineState* pipelineState)
{
	HRESULT hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, pipelineState, IID_PPV_ARGS(&commandList));
	if (FAILED(hr)) {
		return false;
	}
	commandList->Close();
	return true;
}

void DX12DeviceContext::Destroy()
{
	if (commandList != nullptr) {
		commandList->Release();
		commandList = nullptr;
	}

	if (commandAllocator != nullptr) {
		commandAllocator->Release();
		commandAllocator = nullptr;
	}

	if (device != nullptr) {
		device->Release();
		device = nullptr;
	}
}

#endif