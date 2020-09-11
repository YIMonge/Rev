#ifndef __DX12TEXTURE_H__
#define __DX12TEXTURE_H__
#ifdef _USE_DIRECTX12

#include "DX12Device.h"
#include "revTexture.h"

/// <summary>
/// Texture resource for DX12
/// </summary>
class DX12Texture : public revTexture
{
public:
	DX12Texture();
	virtual ~DX12Texture();

	/// <summary>
	/// Upload texture to VRAM
	/// the method use UpdateSubResource from cpu can access memory to gpu only access memory
	/// </summary>
	/// <param name="device"></param>
	/// <returns></returns>
	virtual bool Upload(revDevice* device);

	/// <summary>
	/// create texture view after upload texture resource
	/// </summary>
	virtual revGraphicsResource* OnUploaded(revDevice* device);

private:
	ID3D12Resource* uploadHeap;
};

#endif
#endif
