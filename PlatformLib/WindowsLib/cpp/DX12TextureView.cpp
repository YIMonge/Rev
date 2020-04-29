#include "DX12TextureView.h"


void DX12TextureView::Create(revDevice* device, const revTexture& texture, D3D12_CPU_DESCRIPTOR_HANDLE* heap)
{
    this->device = device;
    D3D12_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
    shaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    shaderResourceViewDesc.Format = ConvertToDXFormat(texture.GetFormat());
    shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;
    device->GetDevice()->CreateShaderResourceView(texture.GetHandle(), &shaderResourceViewDesc, *heap);
}

void DX12TextureView::Destroy()
{

}