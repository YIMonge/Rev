#ifndef __VULKANTEXTUREVIEW_H__
#define __VULKANTEXTUREVIEW_H__

#include "revTextureView.h"
#include "VulkanDescriptorSet.h"
#include "VulkanSampler.h"

class VulkanTextureView : public revTextureView
{
public:
    VulkanTextureView(){}
    virtual ~VulkanTextureView(){}

    void Create(revDevice* device, const revTexture& texture, const VulkanSampler& sampler,  VulkanDescriptorSet* descriptorSet);
    virtual void Destroy();
private:
    VkImageView resourceView;
    VkDescriptorImageInfo descriptorImageInfo;
};

#endif
