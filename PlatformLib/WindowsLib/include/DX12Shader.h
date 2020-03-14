#ifndef __DX12SHADER_H__
#define __DX12SHADER_H__

#include "DX12DeviceContext.h"
#include "IShader.h"
#include <string>

#ifdef _USE_DIRECTX12

class DX12Shader : public IShader
{
public:
	DX12Shader();
	~DX12Shader();

#ifdef _DEBUG
	virtual bool LoadFromFile(const IDeviceContext& deviceContext, const char* path, SHADER_TYPE shaderType);
#endif

	void Release();

	std::wstring utf8_decode(const std::string &str);

private:
};

#endif
#endif
