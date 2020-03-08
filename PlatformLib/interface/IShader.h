#ifndef __ISHADER_H__
#define __ISHADER_H__

#include "revString.h"
#include "IDeviceContext.h"

enum class SHADER_TYPE : uint32
{
	VERTX,
	FRAGMENT,
	MAX_NUM,
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
	revShaderHandle GetHandle() const { return handle; }
protected:
	revString name;
    SHADER_TYPE type;
	revShaderHandle handle;
    REV_GRAPHICS_BUFFER_FOMAT_FLAG dataFormat;
};

#endif