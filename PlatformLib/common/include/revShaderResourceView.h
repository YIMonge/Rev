#ifndef __REVSHADERRESOURCEVIEW_H__
#define __REVSHADERRESOURCEVIEW_H__

#include "revGraphicsTypedef.h"
#include "revDevice.h"
#include "revTexture.h"

#define MEM_ALIGN(N, ALIGN) (N +(ALIGN-1)) & ~(ALIGN-1) 

class revShaderResourceView
{
public:
	revShaderResourceView(){}
	virtual ~revShaderResourceView(){}

	virtual void Destroy() = 0;
protected:
	revDevice* device;
};

#endif

