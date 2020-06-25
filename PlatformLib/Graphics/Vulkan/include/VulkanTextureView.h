#ifndef __VULKANTEXTUREVIEW_H__
#define __VULKANTEXTUREVIEW_H__
#ifdef _USE_VULKAN

#include "revShaderResourceView.h"
#include "VulkanDescriptorSet.h"
#include "VulkanSampler.h"

class VulkanTextureView : public revShaderResourceView
{
public:
    VulkanTextureView(){}
    virtual ~VulkanTextureView(){}

    void Create(revDevice* device, const revTexture& texture, const VulkanSampler& sampler, VulkanDescriptorSet::Chunk& chunk);
    virtual void Destroy();
private:
    VkImageView resourceView;
    VkDescriptorImageInfo descriptorImageInfo;
};

#endif
#endif
