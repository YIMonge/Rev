#ifndef __REVDEVICE_H__
#define __REVDEVICE_H__

#include "revTypedef.h"
#include "revGraphicsTypedef.h"
class revDescriptorHeap;

class revDevice
{
public:
	virtual ~revDevice(){}

	const revGraphicsDevice& GetDevice() const { return device; }
	const revGraphicsAdapter& GetAdapter() const { return adapter; }
	const revGraphicsCommandQueue& GetQueue() const { return queue; }
	const GraphicsDesc& GetDesc() const { return desc; }

	revDescriptorHeap* GetDescriptorHeap(DESCRIPTOR_HEAP_TYPE type) const
	{
		return descriptorHeap[static_cast<uint32>(type)];
	}
protected:
	revGraphicsDevice device;
	revGraphicsAdapter adapter;
	revGraphicsCommandQueue queue;
	GraphicsDesc desc;

	revDescriptorHeap* descriptorHeap[DESCRIPTOR_HEAP_TYPE_MAX_NUM];

};

#endif