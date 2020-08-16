#ifndef __REVGRAPHICSRESOURCE_H__
#define __REVGRAPHICSRESOURCE_H__

#include "revGraphicsTypedef.h"
#include "revDevice.h"

// TODO: refCount
class revGraphicsResource
{
public:
	revGraphicsResource(revDevice* device) : device(device)
	{}
	virtual ~revGraphicsResource(){}

	virtual void Destroy() = 0;
protected:
	revDevice* device;
};

#endif

