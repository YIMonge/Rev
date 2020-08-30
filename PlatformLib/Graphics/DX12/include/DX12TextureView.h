#ifndef __DX12TEXTUREVIEW_H__
#define __DX12TEXTUREVIEW_H__
#ifdef _USE_DIRECTX12

#include "revGraphicsResource.h"
#include "DX12DescriptorHeap.h"
#include "revTexture.h"

class DX12TextureView : public revGraphicsResource
{
public:
	DX12TextureView(revDevice* device) : revGraphicsResource(device) {}
	virtual ~DX12TextureView(){}
	
	void Create(revDevice* device, const revTexture* texture, D3D12_CPU_DESCRIPTOR_HANDLE* heap);
	virtual void Destroy();
private:
	
};

#endif
#endif