#ifndef __REVSAMPLER_H__
#define __REVSAMPLER_H__

#include "revGraphicsTypedef.h"
#include "revTexture.h"

class revSampler
{
public:
    revSampler():
        device(nullptr)
    {}
    virtual ~revSampler(){}

protected:
    revDevice* device;
    revTextureSampler sampler;
    revTexture::SamplerDesc desc;
};
#endif
