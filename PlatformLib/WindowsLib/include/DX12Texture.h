#ifndef __DX12TEXTURE_H__
#define __DX12TEXTURE_H__

#include "DX12Device.h"
#include "revTexture.h"

class DX12Texture : public revTexture
{
public:
	DX12Texture();
	virtual ~DX12Texture();

protected:
	virtual bool CreateTexture(revDevice* device, uint8* imageData);
};

#endif
