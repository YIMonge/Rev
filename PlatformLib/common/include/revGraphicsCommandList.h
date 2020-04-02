#ifndef __REVGRAPHICSCOMMANDLIST_H__
#define __REVGRAPHICSCOMMANDLIST_H__

#include "revGraphicsTypedef.h"
#include "revDevice.h"

class revGraphicsCommandList
{
public:

	virtual bool Create(const revDevice& device) = 0;
public:
	revGraphicsCommandBuffer commandBuffer;
};



#endif
