#include "DX12FrameBuffer.h"

#if defined(_USE_DIRECTX12)


DX12FrameBuffer::DX12FrameBuffer()
{}

DX12FrameBuffer::~DX12FrameBuffer()
{}

bool DX12FrameBuffer::Create(DX12Device& deviceContext, DX12SwapChain& swapChain)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc;
	memset(&desc, 0, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	auto& device = deviceContext.GetDevice();
	desc.NumDescriptors = deviceContext.GetDesc().GetBufferNum();
	descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	return true;
}

void DX12FrameBuffer::Destroy()
{}

#endif
