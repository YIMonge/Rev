#include "DX12Shader.h"

#ifdef _DEBUG
#include "d3dcompiler.h"
#include "revString.h"
#endif

#ifdef _USE_DIRECTX12

DX12Shader::DX12Shader()
{
	handle = nullptr;
}

DX12Shader::~DX12Shader()
{
	Release();
}

#ifdef _DEBUG
std::wstring DX12Shader::utf8_decode(const std::string &str)
{
    if(str.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo( size_needed, 0 );
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

bool DX12Shader::LoadFromFile(const revDeviceContext& deviceContext, const char* path, SHADER_TYPE shaderType)
{	
	std::string resourcePath(RESOURCE_PATH);
	resourcePath += path;
	type = shaderType;
	std::wstring wstr = utf8_decode(resourcePath.c_str());
	HRESULT hr = D3DCompileFromFile(wstr.c_str(),
		nullptr,
		nullptr,
		type == SHADER_TYPE::VERTX ? "VSMain" : "PSMain",
		type == SHADER_TYPE::VERTX ? "vs_5_0" : "ps_5_0",
#ifdef _DEBUG
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
#else
		0,
#endif
		0,
		&handle,
		nullptr
	);

	if (FAILED(hr)) {
		return false;
	}

	return true;
}
#endif

void DX12Shader::Release()
{
	if (handle) {
		handle->Release();
		handle = nullptr;
	}
}

#endif
