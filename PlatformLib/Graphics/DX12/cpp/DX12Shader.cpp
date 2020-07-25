#ifdef _USE_DIRECTX12

#include "DX12Shader.h"

#ifdef _DEBUG
#include <d3dcompiler.h>
#include "revString.h"
#include "Log.h"
#endif


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

bool DX12Shader::LoadFromFile(const revDevice& deviceContext, const char* path, SHADER_TYPE shaderType)
{	
	SetFilePath(path);
	revString resourcePath(RESOURCE_PATH);
	resourcePath += path;
	type = shaderType;
	std::wstring wstr = utf8_decode(resourcePath.c_str());
    ID3DBlob* error = nullptr;
	HRESULT hr = D3DCompileFromFile(wstr.c_str(),
		nullptr,
		nullptr,
		"main", 
		type == SHADER_TYPE::VERTX ? "vs_5_0" : "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&handle,
		&error
	);

	if (FAILED(hr)) {
        if (error != nullptr) {
            const char* errorMsg = reinterpret_cast<const char*>(error->GetBufferPointer());
            NATIVE_LOGE("Shader compile error : %s", errorMsg);
        }
        else NATIVE_LOGE("Shader file not found : ", path);
        if (error != nullptr) error->Release();
		return false;
	}

#if _DEBUG
    if (!LoadMetaData(resourcePath.c_str()) || true) {
        char metaPath[256];
        MakeMetaPath(metaPath, resourcePath.c_str());
        CreateMetaDataFromShader(metaPath);
        LoadMetaData(resourcePath.c_str());
#else
    if (!LoadMetaData(resourcePath.c_str())) {
        NATIVE_LOGE("could not find shader meta data");
        return false;
#endif
    }

    if (error != nullptr) error->Release();
	return true;
}



void DX12Shader::CreateMetaDataFromShader(const char* metaPath)
{
    // Shader reflection for Input Assembler 
    ID3D12ShaderReflection* reflection = nullptr;
    HRESULT hr = D3DReflect(handle->GetBufferPointer(),
        handle->GetBufferSize(),
        IID_ID3D12ShaderReflection,
        (void**)&reflection);
    if (FAILED(hr)) {
        NATIVE_LOGE("Could not get shader reflection");
        return;
    }
    D3D12_SHADER_DESC shaderDesc;
    reflection->GetDesc(&shaderDesc);

    // Input assembler 
    metaData.attributes.clear();
    metaData.attributes.resize(shaderDesc.InputParameters);
    revArray<D3D12_INPUT_ELEMENT_DESC> inputLayouts;
    uint32 offset = 0;
    for (uint32 i = 0; i < shaderDesc.InputParameters; ++i) {
        D3D12_SIGNATURE_PARAMETER_DESC signatureDesc;
        reflection->GetInputParameterDesc(i, &signatureDesc);

        INPUT_ELEMENT_TYPE elementType = ConvertToREVSemantic(signatureDesc.SemanticName);
        metaData.attributes[i].SetInputElementType(elementType);
        metaData.attributes[i].SetFormat(GRAPHICS_SEMANTICS[static_cast<uint32>(elementType)].format);
        metaData.attributes[i].SetLocation(i);
        metaData.attributes[i].SetBinding(signatureDesc.SemanticIndex);
        metaData.attributes[i].SetOffset(offset);
        offset += GRAPHICS_SEMANTICS[static_cast<uint32>(elementType)].sizeOfBytes;
    }

    // cbuffer 
    metaData.cbuffers.clear();
    metaData.cbuffers.resize(shaderDesc.ConstantBuffers);
    for (uint32 i = 0; i < shaderDesc.ConstantBuffers; ++i) {
        auto cbuffer = reflection->GetConstantBufferByIndex(i);
        D3D12_SHADER_BUFFER_DESC bufferDesc;
        cbuffer->GetDesc(&bufferDesc);
        revConstantBufferBinding cbufferBinding;
        cbufferBinding.SetRegisterIndex(i);
		cbufferBinding.SetName(bufferDesc.Name);
        uint32 sizeOfBytes = 0;
        for (uint32 j = 0; j < bufferDesc.Variables; ++j) {
            auto variable = cbuffer->GetVariableByIndex(j);
            D3D12_SHADER_VARIABLE_DESC variableDesc;
            variable->GetDesc(&variableDesc);
            sizeOfBytes += variableDesc.Size;
        }
        metaData.cbuffers[i] = cbufferBinding;
        /*
        for (uint32 j = 0; j < bufferDesc.Variables; ++j) {
            auto variable = cbuffer->GetVariableByIndex(j);
            D3D12_SHADER_VARIABLE_DESC variableDesc;
            variable->GetDesc(&variableDesc);

            revConstantBufferBinding cbuffer;
            cbuffer.SetRegisterIndex(j);
            cbuffer.SetSizeOfBytes(variableDesc.Size);
            cbuffer.
            metaData.cbuffers[i]= cbuffer;
        }
        */

    }

    // Resources 
    metaData.textures.clear();    
    metaData.samplers.clear();
    for (uint32 i = 0; i < shaderDesc.BoundResources; ++i) {
        D3D12_SHADER_INPUT_BIND_DESC bindDesc;
        reflection->GetResourceBindingDesc(i, &bindDesc);
        switch (bindDesc.Type) {
        case D3D_SIT_SAMPLER: 
        {
            revSamplerBinding sampler;
            sampler.SetRegisterIndex(bindDesc.BindPoint);
            sampler.SetCount(bindDesc.BindCount);
            sampler.SetSpace(bindDesc.Space);
            metaData.samplers.push_back(sampler);
        }
            break;

        case D3D_SIT_TEXTURE:
        {  
            revTextureBinding texture;
            texture.SetRegisterIndex(bindDesc.BindPoint);
            texture.SetCount(bindDesc.BindCount);
            texture.SetSpace(bindDesc.Space);
            texture.Set3d(bindDesc.Dimension == D3D10_1_SRV_DIMENSION_TEXTURE3D);
            metaData.textures.push_back(texture);
        }
            break;
        }
    }

    // Load sammpler
    //metaData.samplers.clear();
    //metaData.samplers.resize(shaderDesc.sa)

    revSerializer::Serialize(metaPath, static_cast<revShader&>(*this));

    reflection->Release();
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
