#ifndef __VULKANDESCRIPTORSETLAYOUT_H__
#define __VULKANDESCRIPTORSETLAYOUT_H__
#ifdef _USE_VULKAN

#include "VulkanDevice.h"
#include "revDescriptorBindingDesc.h"
#include "revMaterial.h"

class VulkanDescriptorSetLayout
{
public:
    VulkanDescriptorSetLayout() {}
    virtual ~VulkanDescriptorSetLayout(){}

    bool Create(revDevice* device, const revDescriptorBindingDesc& desc);
    void Destroy();
    
    uint32 GetDescriptorSetCount() const { return static_cast<uint32>(descriptorSetLayouts.size()); }
    const VkDescriptorSetLayout* GetDescriptorSets() const { return descriptorSetLayouts.data(); }
private:
    revDevice* device;
    revArray<VkDescriptorSetLayout> descriptorSetLayouts;
};

#endif
#endif
