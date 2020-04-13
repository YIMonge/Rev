#ifndef __REVSHADER_H__
#define __REVSHADER_H__

#include "revDevice.h"
#include "revResource.h"

enum class SHADER_TYPE : uint32
{
	VERTX,
	FRAGMENT,
	COMPUTE,
	MAX_NUM,
};

// rev use vertex only buffer for optimization
// quotes: galaxy game dev
class revAttributeBinding
{
public:
    revAttributeBinding(){}

	INPUT_ELEMENT_TYPE GetInputElementType() const { return static_cast<INPUT_ELEMENT_TYPE>(type); }
    uint8 GetBinding() const { return binding; }
    uint8 GetLocation() const { return location; }
    GRAPHICS_FORMAT GetForamt() const { return static_cast<GRAPHICS_FORMAT>(format); }
    uint8 GetOffset() const { return offset; }

	SERIALIZE_FUNC()
	{
		archive(REV_NVP(type),
			REV_NVP(binding),
			REV_NVP(location),
			REV_NVP(format),
			REV_NVP(offset)
		);
	}

private:
	uint8 type;
    uint8 binding;
    uint8 location;
    uint8 format;
    uint8 offset;
};



class revShader : public revResource
{
public:
	revShader() :
	name("")
	{
	}
	virtual ~revShader(){}
	virtual bool LoadFromFile(const revDevice& deviceContext, const char* path, SHADER_TYPE shaderType) = 0;
	revShaderHandle GetHandle() const { return handle; }
	const revArray<revAttributeBinding>& GetAttributes() const { return metaData.attributes; }

	struct ShaderMetaData
	{
		DefaultMetaData data;
		revArray<revAttributeBinding> attributes;
	};
	
protected:
	revString name;
    SHADER_TYPE type;
	revShaderHandle handle;
	ShaderMetaData metaData;
    
};


SERIALIZE_FUNC_NON_INTRUSIVE(revShader::ShaderMetaData, metaData)
{
	archive(REV_MAKE_NVP(data, metaData.data),
		REV_MAKE_NVP(attributes, metaData.attributes));
}

#endif