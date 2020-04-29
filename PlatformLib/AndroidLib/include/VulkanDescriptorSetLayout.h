#ifndef __VULKANDESCRIPTORSETLAYOUT_H__
#define __VULKANDESCRIPTORSETLAYOUT_H__

#include "revMaterial.h"
#include "revDevice.h"
#include "revArray.h"
#include "revDescriptorBindingDesc.h"

class VulkanDescriptorSetLayout
{
public:
    VulkanDescriptorSetLayout(){}
    virtual ~VulkanDescriptorSetLayout(){}

    bool Create(revDevice* device, const revDescriptorBindingDesc& desc);
    void Destroy();

private:
    revDevice* device;
};

#endif

