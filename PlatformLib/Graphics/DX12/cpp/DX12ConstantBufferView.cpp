#ifdef _USE_DIRECTX12

#include "DX12ConstantBufferView.h"


void DX12ConstantBufferView::Create(const revGraphicsBuffer* cbuffer, const D3D12_CPU_DESCRIPTOR_HANDLE& heap)
{
    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
    auto bufferHandlePtr = cbuffer->GetHandlePtr();
    cbvDesc.BufferLocation = (*bufferHandlePtr)->GetGPUVirtualAddress();
    cbvDesc.SizeInBytes = MEM_ALIGN((cbuffer->GetSizeOfBytes()*cbuffer->GetLength()), 256);
    device->GetDevice()->CreateConstantBufferView(&cbvDesc, heap);
}

void DX12ConstantBufferView::Destroy()
{
    // TODO:
}

#endif
