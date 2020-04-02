#ifndef __DX12TEXTUREVIEW_H__
#define __DX12TEXTUREVIEW_H__

#include "revTextureView.h"

class DX12TextureView : public revTextureView
{
public:
	DX12TextureView() {}
	virtual ~DX12TextureView(){}
	virtual void Create(const revDevice& deviceContext, const revTexture& texture, revGraphicsHeap* heap);
};

#endif
