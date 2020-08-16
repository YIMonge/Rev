#ifdef _USE_DIRECTX12

#include "DX12VertexBufferView.h"


void DX12VertexBufferView::Create(const revGraphicsBuffer* buffer)
{
	auto handlePtr = buffer->GetHandlePtr();
	view.BufferLocation = (*handlePtr)->GetGPUVirtualAddress();
	view.StrideInBytes = buffer->GetSizeOfBytes(); 
	view.SizeInBytes = buffer->GetSizeOfBytes() * buffer->GetLength();
}

void DX12VertexBufferView::Destroy()
{
	
}
#endif