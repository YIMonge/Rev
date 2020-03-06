#ifndef __ISHADER_H__
#define __ISHADER_H__

#include "revString.h"
#include "IDeviceContext.h"

enum class SHADER_TYPE : uint32
{
	VERTX,
	FRAGMENT,
};

class IShader
{
public:
	IShader() :
	name("")
	{
	}
	virtual ~IShader(){}

	virtual bool LoadFromFile(const IDeviceContext& deviceContext, const char* path, SHADER_TYPE shaderType) = 0;

protected:
	revString name;
    SHADER_TYPE type;
	revShaderHandle handle;
    REV_GRAPHICS_BUFFER_FOMAT_FLAG dataFormat;
};

#endif