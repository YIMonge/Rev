#ifndef __REVDEVICECONTEXT_H__
#define __REVDEVICECONTEXT_H__

#include "revTypedef.h"
#include "revGraphicsTypedef.h"

class revDeviceContext
{
public:
	virtual ~revDeviceContext(){}

	const revGraphicsDevice& GetDevice() const { return device; }
	const revGraphicsCommandQueue& GetQueue() const { return queue; }
	const GraphicsDesc& GetDesc() const { return desc; }

protected:
	revGraphicsDevice device;
	revGraphicsCommandQueue queue;
	GraphicsDesc desc;
};

#endif