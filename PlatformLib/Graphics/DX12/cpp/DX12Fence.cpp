#include "DX12Fence.h"
#include "Log.h"

DX12Fence::DX12Fence(DX12Device* device) :
	device(device),
	fence(nullptr),
	fenceValue(0)
{

}

DX12Fence::~DX12Fence()
{

}

bool DX12Fence::Create()
{
	// create fence 
	HRESULT hr = device->GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
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

void DX12Fence::Destroy()
{

}

void DX12Fence::Reset()
{

}

void DX12Fence::WaitForQueue()
{
	ID3D12CommandQueue* queue = device->GetQueue();
	const uint64 tempFenceValue = fenceValue;
	HRESULT hr = queue->Signal(fence, tempFenceValue);
	if (FAILED(hr)) {
		NATIVE_LOGE("failed to set fence to command queue");
		return;
	}
	fenceValue++;
	if (fence->GetCompletedValue() < tempFenceValue) {
		hr = fence->SetEventOnCompletion(tempFenceValue, fenceEvent);
		if (FAILED(hr)) {
			NATIVE_LOGE("failed to set event to fence");
			return;
		}
		WaitForSingleObject(fenceEvent, INFINITE);
	}
}
