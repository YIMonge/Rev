#include "DX12CommandList.h"
#include "Log.h"

bool DX12CommandList::Create(revDevice* device, revGraphicsPipeline* pipeline)
{
	if (device == nullptr || device->GetDevice() == nullptr) return false;

	this->device = device;
	auto& dxDevice = device->GetDevice();

	// create allocator and command list 
	HRESULT hr = dxDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	if (FAILED(hr)) {
		NATIVE_LOGE("failed to create command allocator");
		return false;
	}

	hr = dxDevice->CreateCommandList(0, 
		D3D12_COMMAND_LIST_TYPE_DIRECT, 
		commandAllocator, 
		pipeline == nullptr ? nullptr : *pipeline, 
		IID_PPV_ARGS(&commandBuffer));
	if (FAILED(hr)) {
		NATIVE_LOGE("failed to create command list");
		return false;
	}
	return true;
}

void DX12CommandList::Open()
{
	needBarrierResources.clear();
	HRESULT hr = commandAllocator->Reset();
	if (FAILED(hr)) {
		NATIVE_LOGE("failed to reset command allocator");
		return;
	}
	hr = commandBuffer->Reset(commandAllocator, nullptr);
	if (FAILED(hr)) {
		NATIVE_LOGE("failed to reset command list");
		return;
	}
}

void DX12CommandList::Close()
{
	if (!needBarrierResources.empty()) {
		commandBuffer->ResourceBarrier(needBarrierResources.size(), needBarrierResources.data());
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
	uint32 length = needReleaseResources.size();
	for (uint32 i = 0; i < length; ++i) {
		needReleaseResources[i]->Release();
	}
	needReleaseResources.clear();
}