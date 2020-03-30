#ifndef __DX12TEXTURE_H__
#define __DX12TEXTURE_H__

#include "DX12DeviceContext.h"
#include "revTexture.h"

class DX12Texture : public revTexture
{
public:
	DX12Texture();
	virtual ~DX12Texture();

protected:
	virtual bool CreateTexture(const revDeviceContext& deviceContext, uint8* imageData);

private:
};

#endif
