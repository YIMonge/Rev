#ifdef _USE_DIRECTX12

#include "DX12ConstantBufferView.h"


void DX12ConstantBufferView::Create(const revGraphicsBuffer* cbuffer, revDescriptorHeap* descriptorHeap, revDescriptorHeap::Chunk* allocatedChunk, uint32 offset)
{
	this->chunkOffset = offset;
	this->descriptorHeap = descriptorHeap;
	this->chunk = allocatedChunk;
	if (chunk == nullptr) {
		if (offset > 0) {
			NATIVE_LOGE("Cant allocate chunk.");
			return;
		}
		chunk = descriptorHeap->Allocation();
	}
    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
    auto bufferHandlePtr = cbuffer->GetHandlePtr();
    cbvDesc.BufferLocation = (*bufferHandlePtr)->GetGPUVirtualAddress();
    cbvDesc.SizeInBytes = MEM_ALIGN((cbuffer->GetSizeOfBytes()*cbuffer->GetLength()), 256);
    device->GetDevice()->CreateConstantBufferView(&cbvDesc, chunk->GetHandle(chunkOffset));
}

void DX12ConstantBufferView::Destroy()
{
	if (descriptorHeap != nullptr && chunk != nullptr) {
		descriptorHeap->Free(chunk, chunkOffset);
	}	
}

#endif
