#ifdef _USE_DIRECTX12

#include "../include/DX12Device.h"
#include "../include/Window.h"
#include "revTypedef.h"
#include "Log.h"


bool DX12Device::Create(const GraphicsDesc& desc)
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
		NATIVE_LOGE("failed to create device. File[%s], line[%d]", __FILE__, __LINE__);
		return false;
	}

	// Create the command queue.
	D3D12_COMMAND_QUEUE_DESC queueDesc;
	memset(&queueDesc, 0, sizeof(queueDesc));

	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	hr = device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&queue));
	if (FAILED(hr)) {
		NATIVE_LOGE("failed to create command queue. File[%s], line[%d]", __FILE__, __LINE__);
		return false;
	}

	globalCommandList.Create(this, nullptr);
	return true;
}


void DX12Device::Destroy()
{
	if (queue != nullptr) {
		queue->Release();
		queue = nullptr;
	}

	if (device != nullptr) {
		device->Release();
		device = nullptr;
	}
}

#endif