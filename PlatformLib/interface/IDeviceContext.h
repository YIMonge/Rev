#ifndef __IDEVICECONTEXT_H__
#define __IDEVICECONTEXT_H__

#include "revTypedef.h"
#include "GraphicsTypedef.h"

class IDeviceContext
{
public:
	virtual ~IDeviceContext(){}

	const revGraphicsDevice& GetDevice() const
	{
		return device;
	}

	const revGraphicsCommandQueue& GetQueue() const
	{
		return queue;
	}
protected:
	revGraphicsDevice device;
	revGraphicsCommandQueue queue;
};

#endif