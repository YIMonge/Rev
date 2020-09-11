#ifndef __REVGRAPHICSRESOURCE_H__
#define __REVGRAPHICSRESOURCE_H__

#include "revGraphicsTypedef.h"
#include "revDevice.h"
#include "revDescriptorHeap.h"

// TODO: refCount
class revGraphicsResource
{
public:
	revGraphicsResource(revDevice* device) : 
		device(device),
		chunk(nullptr),
		chunkOffset(0)
	{}
	virtual ~revGraphicsResource(){}
	virtual void Destroy() = 0;

	revDescriptorCPUHandle GetHandle() const
	{
		if (chunk == nullptr) {
			// TODO:Assert
			revDescriptorCPUHandle errorHandle;
			errorHandle.ptr = NULL_HANDLE;
			return errorHandle;
		}
		return chunk->GetHandle(chunkOffset);
	}

	uint32 GetOffset() const
	{
		return chunkOffset;
	}

protected:	
	revDevice* device;
	revDescriptorHeap::Chunk* chunk;
	uint32 chunkOffset;
};


#endif

