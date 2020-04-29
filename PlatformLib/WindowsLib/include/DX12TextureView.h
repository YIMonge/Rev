#ifndef __DX12TEXTUREVIEW_H__
#define __DX12TEXTUREVIEW_H__

#include "revTextureView.h"
#include "DX12DescriptorHeap.h"

class DX12TextureView : public revTextureView
{
public:
	DX12TextureView() {}
	virtual ~DX12TextureView(){}
	
	void Create(revDevice* device, const revTexture& texture, D3D12_CPU_DESCRIPTOR_HANDLE* heap);
	virtual void Destroy();
private:
	
};

#endif
