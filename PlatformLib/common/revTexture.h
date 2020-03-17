#ifndef __REVTEXTURE_H__
#define __REVTEXTURE_H__

#include "revGraphicsTypedef.h"

class revTexture
{
public:
    revTexture(){}
    virtual ~revTexture(){}

    const revTextureSampler& GetSampler() const { return sampler; }
    const revTextureResourceView& GetResourceView() const { return resourceView; }
protected:
    revTextureSampler sampler;
    revTextureResourceView  resourceView;
};

#endif