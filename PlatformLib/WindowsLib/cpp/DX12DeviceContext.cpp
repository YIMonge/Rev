#include "../include/DX12DeviceContext.h"
#include "../include/Window.h"
#include "revTypedef.h"
#include "revSmartPointer.h"


#if defined(_USE_DIRECTX12)

bool DX12DeviceContext::Create(Window* window, const GraphicsDesc& desc)
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

	//CreateDXGIFactory1(IID_PPV_ARGS(&dxgi_factroy));

	// Create the Direct3D 12 API device object
	HRESULT hr = D3D12CreateDevice(
		nullptr,						// Specify nullptr to use the default adapter.
		D3D_FEATURE_LEVEL_11_0,			// Minimum feature level this app can support.
		IID_PPV_ARGS(&device)			// Returns the Direct3D device created.
		);

	// Create the command queue.
	D3D12_COMMAND_QUEUE_DESC queueDesc;
	memset(&queueDesc, 0, sizeof(queueDesc));

	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&command_queue));


	uint32 buffer_num = 2;
	if (desc.buffer_type == GraphicsDesc::BUFFERTYPE::TRIPLE_BUFFER) {
		buffer_num = 3;
	}

	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocator));

	// Create synchronization objects.
	//device->CreateFence(m_fenceValues[m_currentFrame], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
	//m_fenceValues[m_currentFrame]++;

	//m_fenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);

	return true;
}

#endif