#ifndef __REVDEVICECONTEXT_H__
#define __REVDEVICECONTEXT_H__

#include "revTypedef.h"
#include "revGraphicsTypedef.h"

class revDeviceContext
{
public:
	virtual ~revDeviceContext(){}

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