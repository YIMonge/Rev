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
		R8B8G8A8_SRGB,
		BUFFERFORMAT_MAX_NUM,
	};

	BUFFERTYPE		buffer_type;
	BUFFERFORMAT	buffer_format;
	bool			use_fullscreen;
};

class IDeviceContext
{
public:
	virtual ~IDeviceContext(){}
};

#endif