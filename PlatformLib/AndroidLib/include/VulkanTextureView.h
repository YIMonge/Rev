#ifndef __VULKANTEXTUREVIEW_H__
#define __VULKANTEXTUREVIEW_H__

#include "revTextureView.h"

class VulkanTextureView : public revTextureView
{
public:
    VulkanTextureView(){}
    virtual ~VulkanTextureView(){}

    virtual void Create(const revDevice& device, const revTexture& texture, revGraphicsHeap* heap = nullptr);

    const VkDescriptorImageInfo& GetDescriptorImageInfo() const { return descriptorImageInfo; }
private:
    VkDescriptorImageInfo descriptorImageInfo;
};

#endif
