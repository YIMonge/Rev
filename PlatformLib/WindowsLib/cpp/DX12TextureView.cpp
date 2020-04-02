#include "DX12TextureView.h"


void DX12TextureView::Create(const revDevice& deviceContext, const revTexture& texture, revGraphicsHeap* heap)
{
    D3D12_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
    shaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    shaderResourceViewDesc.Format = ConvertToDXFormat(texture.GetFormat());
    shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;

    auto device = deviceContext.GetDevice();
    device->CreateShaderResourceView(texture.GetHandle(), &shaderResourceViewDesc, heap->GetCPUDescriptorHandleForHeapStart());
}