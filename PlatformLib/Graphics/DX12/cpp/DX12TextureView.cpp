#ifdef _USE_DIRECTX12

#include "DX12TextureView.h"


void DX12TextureView::Create(const revTexture* texture, revDescriptorHeap* descriptorHeap, revDescriptorHeap::Chunk* allocatedChunk, uint32 offset)
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

    D3D12_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
    shaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    shaderResourceViewDesc.Format = ConvertToDXFormat(texture->GetFormat());
    shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;
    device->GetDevice()->CreateShaderResourceView(texture->GetHandle(), &shaderResourceViewDesc, chunk->GetHandle(offset));
}

void DX12TextureView::Destroy()
{
	if (descriptorHeap != nullptr && chunk != nullptr) {
		descriptorHeap->Free(chunk, chunkOffset);
	}
}

#endif
