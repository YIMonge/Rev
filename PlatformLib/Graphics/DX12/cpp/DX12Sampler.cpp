#ifdef _USE_DIRECTX12

#include "DX12Sampler.h"

bool DX12Sampler::Create(DX12Device* device, const revTexture* texture, D3D12_CPU_DESCRIPTOR_HANDLE* heap)
{
	this->device = device;
	auto& dxDevice = device->GetDevice();
	desc = texture->GetSamplerDesc();
	D3D12_SAMPLER_DESC dxDesc;
	memset(&dxDesc, 0, sizeof(dxDesc));
	dxDesc.AddressU = ConverToDXTextureAddressMode(desc.GetAddressModeU());
	dxDesc.AddressV = ConverToDXTextureAddressMode(desc.GetAddressModeV());
	dxDesc.AddressW = ConverToDXTextureAddressMode(desc.GetAddressModeW());
	dxDesc.ComparisonFunc = ConvertToDXComparisonFunc(desc.GetComparisonFunc());
	dxDesc.Filter = ConvertToDXFilter(desc.GetMinFilter(), desc.GetMagFilter(), desc.GetMipFilter(), desc.GetMaxAnisotropy(), COMPARISON_FUNC::NEVER != desc.GetComparisonFunc());
	dxDesc.MaxAnisotropy = desc.GetMaxAnisotropy();
	dxDesc.MaxLOD = desc.GetMaxLod();
	dxDesc.MinLOD = desc.GetMinLod();
	dxDesc.MipLODBias = desc.GetMipLodBias();
	revColor borderColor = desc.GetBorderColor() == BORDER_COLOR_MODE::OPAQUE_BLACK ? revColor::BLACK : revColor::WHITE;
	for (uint32 i = 0; i < 4; ++i) {
		dxDesc.BorderColor[i] = borderColor.data[i];
	}
	dxDevice->CreateSampler(&dxDesc, *heap);
	return true;
}

void DX12Sampler::Destroy()
{

}
#endif

