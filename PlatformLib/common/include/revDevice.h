#ifndef __REVDEVICE_H__
#define __REVDEVICE_H__

#include "revTypedef.h"
#include "revGraphicsTypedef.h"

class revDevice
{
public:
	virtual ~revDevice(){}

	const revGraphicsDevice& GetDevice() const { return device; }
	const revGraphicsAdapter& GetAdapter() const { return adapter; }
	const revGraphicsCommandQueue& GetQueue() const { return queue; }
	const GraphicsDesc& GetDesc() const { return desc; }

protected:
	revGraphicsDevice device;
	revGraphicsAdapter adapter;
	revGraphicsCommandQueue queue;
	GraphicsDesc desc;
};

#endif