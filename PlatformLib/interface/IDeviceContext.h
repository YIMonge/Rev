#ifndef __IDEVICECONTEXT_H__
#define __IDEVICECONTEXT_H__

#include "revTypedef.h"

struct GraphicsDesc
{
	enum BUFFERTYPE
	{
		DOUBLE_BUFFER = 0,
		TRIPLE_BUFFER,
		BUFFERTYPE_MAX_NUM,
	};

	enum BUFFERFORMAT
	{
		R8B8G8A8,
		BUFFERFORMAT_MAX_NUM,
	};

	BUFFERTYPE		bufferType;
	BUFFERFORMAT	bufferFormat;
	bool			useFullscreen;

	uint32 GetBufferNum() const
	{
		return bufferType == BUFFERTYPE::DOUBLE_BUFFER ? 2 : 3;
	}
};

class IDeviceContext
{
public:
	virtual ~IDeviceContext(){}
};

#endif