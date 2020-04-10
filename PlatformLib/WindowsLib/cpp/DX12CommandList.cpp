#include "DX12CommandList.h"

bool DX12CommandList::Create(revDevice* device, revGraphicsPipeline* pipeline)
{
	if (device == nullptr || device->GetDevice() == nullptr) return false;

	this->device = device;
	auto dxDevice = device->GetDevice();

	// create allocator and command list 
	HRESULT hr = dxDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	if (FAILED(hr)) {
		return false;
	}

	hr = dxDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, *pipeline, IID_PPV_ARGS(&commandBuffer));
	if (FAILED(hr)) {
		return false;
	}
	return true;
}

void DX12CommandList::Open()
{
	HRESULT hr = commandAllocator->Reset();
	if (FAILED(hr)) return;
	hr = commandBuffer->Reset(commandAllocator, nullptr);
	if (FAILED(hr)) return;
}

void DX12CommandList::Close()
{
	commandBuffer->Close();
}

void DX12CommandList::SetPipelineState(revGraphicsPipeline& pipeline)
{

}
