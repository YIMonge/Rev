#ifndef __DX12SAMPLER_H__
#define __DX12SAMPLER_H__

#include "revSampler.h"
#include "DX12Texture.h"

class DX12Sampler : public revSampler
{
public:
	bool Create(DX12Device* device, const revTexture& texture, D3D12_CPU_DESCRIPTOR_HANDLE* heap);
	void Destroy();
};

#endif
