#ifdef _USE_DIRECTX12

#include "DX12Device.h"
#include "Window.h"
#include "DX12DescriptorHeap.h"
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

	uint32 dxgiFlags = 0;
#ifdef _DEBUG
	dxgiFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
	hr = CreateDXGIFactory2(dxgiFlags, IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(hr)) {
		if (dxgiFactory != nullptr) dxgiFactory->Release();
		NATIVE_LOGE("failed to create dxgi factory");
		return false;
	}


	// TODO: set by parameter settings
	uint32 descriptorNums[DESCRIPTOR_HEAP_TYPE_MAX_NUM] =
	{
		1024,
		1024,
		1024,
		desc.GetBufferNum(),
		desc.GetBufferNum()
	};

	// TODO: set by parameter settings
	bool descriptorShaderVisiblity[DESCRIPTOR_HEAP_TYPE_MAX_NUM] =
	{
		true,
		true,
		true,
		false,
		false
	};

	for (uint32 i = 0; i < DESCRIPTOR_HEAP_TYPE_MAX_NUM; ++i) {
		descriptorHeap[i] = new DX12DescriptorHeap(this);
		descriptorHeap[i]->Create(static_cast<DESCRIPTOR_HEAP_TYPE>(i), descriptorNums[i], descriptorShaderVisiblity[i]);
	}

	return true;
}


void DX12Device::Destroy()
{
	for (uint32 i = 0; i < DESCRIPTOR_HEAP_TYPE_MAX_NUM; ++i) {
		if (descriptorHeap[i] != nullptr) {
			descriptorHeap[i]->Destroy();
			delete descriptorHeap[i];
		}
	}

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