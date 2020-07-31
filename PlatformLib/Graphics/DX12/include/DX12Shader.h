#ifndef __DX12SHADER_H__
#define __DX12SHADER_H__
#ifdef _USE_DIRECTX12

#include "DX12Device.h"
#include "revShader.h"
#include <string>

class DX12Shader : public revShader
{
public:
	DX12Shader();
	~DX12Shader();

	virtual bool LoadFromFile(const revDevice* device, const char* path, SHADER_TYPE shaderType);
	void Release();

	std::wstring utf8_decode(const std::string &str);

private:
#ifdef _DEBUG
	void CreateMetaDataFromShader(const char* metaPath);
#endif

};

#endif
#endif
