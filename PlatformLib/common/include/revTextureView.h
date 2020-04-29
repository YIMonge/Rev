#ifndef __REVTEXTUREVIEW_H__
#define __REVTEXTUREVIEW_H__

#include "revGraphicsTypedef.h"
#include "revDevice.h"
#include "revTexture.h"

class revTextureView
{
public:
	revTextureView(){}
	virtual ~revTextureView(){}

	virtual void Destroy() = 0;
protected:
	revDevice* device;
};

#endif

