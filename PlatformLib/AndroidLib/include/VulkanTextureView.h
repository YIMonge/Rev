#ifndef __VULKANTEXTUREVIEW_H__
#define __VULKANTEXTUREVIEW_H__

#include "revTextureView.h"

class VulkanTextureView : public revTextureView
{
public:
    VulkanTextureView() {}
    virtual ~VulkanTextureView(){}
    virtual void Create(const revDeviceContext& deviceContext, const revTexture& texture, revGraphicsHeap* heap);
};

#endif
