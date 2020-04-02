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

	virtual void Create(const revDevice& deviceContext, const revTexture& texture, revGraphicsHeap* heap = nullptr) = 0;
protected:
	revTextureResourceView  resourceView;
};

#endif

