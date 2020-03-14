#ifndef __DX12SHADER_H__
#define __DX12SHADER_H__

#include "DX12DeviceContext.h"
#include <string>

#ifdef _USE_DIRECTX12

// TODO: merge interface into vulkan shader 
enum class SHADER_TYPE : uint32
{
	VERTX,
	FRAGMENT,
};

class DX12Shader
{
public:
	DX12Shader();
	~DX12Shader();

#ifdef _DEBUG
	bool LoadFromFile(const DX12DeviceContext& deviceContext, const char* path, SHADER_TYPE shaderType);
#endif

	void Release();

	ID3DBlob* GetHandle() const
	{
		return handle;
	}

	std::wstring utf8_decode(const std::string &str);

private:
	SHADER_TYPE type;
	ID3DBlob* handle;
};

#endif
#endif
