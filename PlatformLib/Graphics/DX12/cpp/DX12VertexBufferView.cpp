#ifdef _USE_DIRECTX12

#include "DX12VertexBufferView.h"


void DX12VertexBufferView::Create(revDevice* device, const DX12VertexBuffer& buffer)
{
    this->device = device;

	auto handlePtr = buffer.GetHandlePtr();
	view.BufferLocation = (*handlePtr)->GetGPUVirtualAddress();
	view.StrideInBytes = buffer.GetSizeOfBytes(); 
	view.SizeInBytes = buffer.GetSizeOfBytes() * buffer.GetLength();
}

void DX12VertexBufferView::Destroy()
{
	
}
#endif