#ifndef __REVRENDERINFO_H__
#define __REVDENDERINFO_H__ 

#include "revDevice.h"
#include "revMaterial.h"

class revRenderInfo
{
public:
	revRenderInfo() {}
	virtual ~revRenderInfo(){}

	virtual void LoadPipeline(const revDevice& device, const revMaterial& material) = 0;

};

#endif 
