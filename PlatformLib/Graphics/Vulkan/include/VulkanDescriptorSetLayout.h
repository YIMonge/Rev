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
    //const DESCRIPTOR_HEAP_TYPE GetDescriptorHeapType(uint32 index) const { return descriptorHeapTypes[index];}
    const VkDescriptorSetLayout* GetDescriptorSets() const { return descriptorSetLayouts.data(); }
    //const VkDescriptorSetLayout* GetDescriptorSets() const { return &descriptorSetLayout; }
private:
    revDevice* device;
    //revArray<DESCRIPTOR_HEAP_TYPE> descriptorHeapTypes;
    //VkDescriptorSetLayout descriptorSetLayout;
    revArray<VkDescriptorSetLayout> descriptorSetLayouts;
};

#endif
#endif
