#ifdef _USE_DIRECTX12

#include "DX12CommandList.h"
#include "Log.h"

bool DX12CommandList::Create(revDevice* device, revGraphicsPipeline* pipeline, revGraphicsCommandAllocator* allocator)
{
	if (device == nullptr || device->GetDevice() == nullptr) return false;

	this->device = device;
	auto& dxDevice = device->GetDevice();

	HRESULT hr;
	if (allocator == nullptr) {
		hr = dxDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
		if (FAILED(hr)) {
			NATIVE_LOGE("failed to create command allocator. File[%s], line[%d]", __FILE__, __LINE__);
			return false;
		}
	}
	else commandAllocator = *allocator;

	hr = dxDevice->CreateCommandList(0, 
		D3D12_COMMAND_LIST_TYPE_DIRECT, 
		commandAllocator, 
		pipeline == nullptr ? nullptr : *pipeline, 
		IID_PPV_ARGS(&commandBuffer));
	if (FAILED(hr)) {
		NATIVE_LOGE("failed to create command list. File[%s], line[%d]", __FILE__, __LINE__);
		return false;
	}
	return true;
}

void DX12CommandList::Open()
{
	needBarrierResources.clear();
	HRESULT hr = commandAllocator->Reset();
	if (FAILED(hr)) {
		NATIVE_LOGE("failed to reset command allocator. File[%s], line[%d]", __FILE__, __LINE__);
		return;
	}
	hr = commandBuffer->Reset(commandAllocator, nullptr);
	if (FAILED(hr)) {
		NATIVE_LOGE("failed to reset command list. File[%s], line[%d]", __FILE__, __LINE__);
		return;
	}
}

void DX12CommandList::Close()
{
	if (!needBarrierResources.empty()) {
		commandBuffer->ResourceBarrier(static_cast<uint32>(needBarrierResources.size()), needBarrierResources.data());
	}
	commandBuffer->Close();
}

void DX12CommandList::AddTransitionBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after, uint32 subresource)
{
	if (!needBarrierResources.empty()) {
		D3D12_RESOURCE_BARRIER last = needBarrierResources[needBarrierResources.size() - 1];
		if (last.Type == D3D12_RESOURCE_BARRIER_TYPE_TRANSITION &&
			last.Transition.StateBefore == before &&
			last.Transition.StateAfter == after &&
			last.Transition.Subresource == subresource) {
			return;
		}
	}

	D3D12_RESOURCE_BARRIER barrier;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.StateBefore = before;
	barrier.Transition.StateAfter = after;
	barrier.Transition.Subresource = subresource;
	barrier.Transition.pResource = resource;

	needBarrierResources.push_back(barrier);
}

void DX12CommandList::ReleaseResoucers()
{
	uint32 length = static_cast<uint32>(needReleaseResources.size());
	for (uint32 i = 0; i < length; ++i) {
		needReleaseResources[i]->Release();
	}
	needReleaseResources.clear();
}

#endif
