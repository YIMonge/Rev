#ifndef __ITEXTURE_H__
#define __ITEXTURE_H__

#include "GraphicsTypedef.h"

class ITexture
{
public:
    ITexture(){}
    virtual ~ITexture(){}

    const revTextureSampler& GetSampler() const { return sampler; }
    const revTextureResourceView& GetResourceView() const { return resourceView; }
protected:
    revTextureSampler sampler;
    revTextureResourceView  resourceView;
};

#endif