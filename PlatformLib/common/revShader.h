#ifndef __revShader_H__
#define __revShader_H__

#include "revDeviceContext.h"
#include "revResource.h"

enum class SHADER_TYPE : uint32
{
	VERTX,
	FRAGMENT,
	MAX_NUM,
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
	const revArray<REV_GRAPHICS_BUFFER_FOMAT_FLAG>& GetFormats() const { return dataFormat; }
protected:
	revString name;
    SHADER_TYPE type;
	revShaderHandle handle;
    revArray<REV_GRAPHICS_BUFFER_FOMAT_FLAG> dataFormat;
};



#endif