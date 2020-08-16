#ifndef __VULKANCONSTANTBUFFERVIEW_H__
#define __VULKANCONSTANTBUFFERVIEW_H__

#include "revGraphicsResource.h"
#include "VulkanDescriptorSet.h"
#include "VulkanBuffer.h"

class VulkanConstantBufferView : public revGraphicsResource
{
public:
    VulkanConstantBufferView(){}
    virtual ~VulkanConstantBufferView(){}

    void Create(revDevice* device, const VulkanConstantBuffer& cbuffer, VulkanDescriptorSet::Chunk& chunk);
    virtual void Destroy();
private:
    VkDescriptorBufferInfo descriptorBufferInfo;

};



#endif

