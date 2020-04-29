#ifndef __DX12SAMPLER_H__
#define __DX12SAMPLER_H__

#include "DX12Texture.h"

class DX12Sampler
{
public:

	bool Create(DX12Device* device, const revTexture& texture, revCPUDescriptorHandle* heap);
	void Destroy();
private:
	DX12Device* device;
	revTextureSampler sampler;
	revTexture::SamplerDesc desc;
};

#endif
