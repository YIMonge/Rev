#ifndef __VULKANSAMPLER_H__
#define __VULKANSAMPLER_H__

#include "VulkanTexture.h"
#include "revSampler.h"

class VulkanSampler : public revSampler
{
public:
    bool Create(VulkanDevice* device, const revTexture& texture);
    void Destroy();
};

#endif
