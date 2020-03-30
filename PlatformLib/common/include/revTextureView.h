#ifndef __REVTEXTUREVIEW_H__
#define __REVTEXTUREVIEW_H__

#include "revGraphicsTypedef.h"
#include "revDeviceContext.h"
#include "revTexture.h"

class revTextureView
{
public:
	revTextureView(){}
	virtual ~revTextureView(){}

	virtual void Create(const revDeviceContext& deviceContext, const revTexture& texture, revGraphicsHeap* heap) = 0;
protected:
	revTextureResourceView  resourceView;
};

#endif

