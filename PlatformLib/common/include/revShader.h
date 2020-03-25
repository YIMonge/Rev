#ifndef __REVSHADER_H__
#define __REVSHADER_H__

#include "revDeviceContext.h"
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

    uint8 GetBinding() const { return binding; }
    uint8 GetLocation() const { return location; }
    GRAPHICS_FORMAT GetForamt() const { return static_cast<GRAPHICS_FORMAT>(format); }
    uint8 GetOffset() const { return offset; }
private:
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
	virtual bool LoadFromFile(const revDeviceContext& deviceContext, const char* path, SHADER_TYPE shaderType) = 0;
	revShaderHandle GetHandle() const { return handle; }
	const revArray<revAttributeBinding>& GetAttributes() const { return attributes; }
protected:
	revString name;
    SHADER_TYPE type;
	revShaderHandle handle;
    revArray<revAttributeBinding> attributes;
};



#endif