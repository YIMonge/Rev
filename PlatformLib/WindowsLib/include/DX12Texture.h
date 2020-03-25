#ifndef __DX12TEXTURE_H__
#define __DX12TEXTURE_H__

#include "DX12DeviceContext.h"
#include "revTexture.h"

class DX12Texture : public revTexture
{
public:
	DX12Texture();
	virtual ~DX12Texture();

	bool LoadFromFile(const DX12DeviceContext& deviceContext, const char* path);
private:
	ID3D12Resource* handle;
	
};

#endif
