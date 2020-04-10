#ifndef __REVGRAPHICSCOMMANDLIST_H__
#define __REVGRAPHICSCOMMANDLIST_H__

#include "revGraphicsTypedef.h"
#include "revDevice.h"

class revGraphicsCommandList
{
public:
	virtual bool Create(revDevice* device, revGraphicsPipeline* pipeline) = 0;
	virtual void Open(){}
	virtual void Close(){}

	revGraphicsCommandAllocator& GetAllocator() { return commandAllocator;  };
	revGraphicsCommandBuffer& GetList() { return commandBuffer; }
protected:
	revDevice* device;
	revGraphicsCommandAllocator commandAllocator;
	revGraphicsCommandBuffer commandBuffer;
};



#endif
