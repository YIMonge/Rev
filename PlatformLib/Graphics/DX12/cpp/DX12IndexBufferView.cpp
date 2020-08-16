#ifdef _USE_DIRECTX12

#include "DX12IndexBufferView.h"


void DX12IndexBufferView::Create(const revGraphicsBuffer* buffer)
{
	auto handlePtr = buffer->GetHandlePtr();
	view.BufferLocation = (*handlePtr)->GetGPUVirtualAddress();
	view.SizeInBytes = buffer->GetSizeOfBytes() * buffer->GetLength();
	view.Format = DXGI_FORMAT_R32_UINT;

	indexCount = buffer->GetLength();
}

void DX12IndexBufferView::Destroy()
{
	
}
#endif