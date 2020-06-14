#ifndef __REVSHADERRESOURCEVIEW_H__
#define __REVSHADERRESOURCEVIEW_H__

#include "revGraphicsTypedef.h"
#include "revDevice.h"
#include "revTexture.h"

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

