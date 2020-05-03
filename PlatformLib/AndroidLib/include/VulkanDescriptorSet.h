#ifndef __VULKANDESCRIPTORSET_H__
#define __VULKANDESCRIPTORSET_H__

#include "VulkanDevice.h"
#include "VulkanDescriptorSetLayout.h"
#include "revMaterial.h"

class VulkanDescriptorSet
{
public:
    VulkanDescriptorSet(){}
    virtual ~VulkanDescriptorSet(){}

    bool Create(revDevice* device, DESCRIPTOR_HEAP_TYPE type, const VulkanDescriptorSetLayout& layout, uint32 numDescriptors, bool shaderVisiblity);
    void Destroy();

    const VkDescriptorSet& GetHandle() const { return descriptorSet; }

private:
    revDevice* device;
    DESCRIPTOR_HEAP_TYPE type;
    uint32 maxDescriptors;
    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSet;
};

#endif

