#ifndef __VULKANDESCRIPTORSETLAYOUT_H__
#define __VULKANDESCRIPTORSETLAYOUT_H__

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

    void Apply(VulkanCommandList& commandList);

    uint32 GetDescriptorSetCount() const { return static_cast<uint32>(descriptorSetLayouts.size()); }
    const VkDescriptorSetLayout* GetDescriptorSet() const { return descriptorSetLayouts.data(); }
private:
    revDevice* device;
    revArray<VkDescriptorSetLayout> descriptorSetLayouts;
};

#endif
