#ifndef __REVDESCRIPTORHEAP_H__
#define __REVDESCRIPTORHEAP_H__

#include "revDevice.h"

class revDescriptorHeap
{
public:
	revDescriptorHeap(revDevice* device) : 
	device(device)
	{}
	virtual ~revDescriptorHeap(){}

	class Chunk
	{
	public:
		Chunk(){}
		virtual revDescriptorCPUHandle GetHandle(uint32 index = 0) const = 0;
	protected:
		revDescriptorCPUHandle handle;
	};

protected:
	revDevice* device;
};

#endif
