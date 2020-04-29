#include "VulkanDescriptorSetLayout.h"

bool VulkanDescriptorSetLayout::Create(revDevice* device, const revDescriptorBindingDesc& desc)
{
    this->device = device;

    uint32 setCount = desc.GetDescriptorSetLayoutCount();
    revArray<VkDescriptorSetLayoutCreateInfo> descriptorSetLayoutInfo(setCount);
    for(uint32 i = 0; i < setCount; ++i){
        auto& set = desc.GetDescriptorSetLayout(i);
        uint32 rangeCount = set.GetRangeCount();
        revArray<VkDescriptorSetLayoutBinding> descriptorBindings(rangeCount);
        for(uint32 j = 0; j < rangeCount; ++j){
            auto& range = set.GetRange(j);
            descriptorBindings[j].binding = range.registerIndex;
            descriptorBindings[j].descriptorType = ConvertToVKDescriptorType(range.type);
            descriptorBindings[j].descriptorCount = range.count;
            descriptorBindings[j].stageFlags = ConvertToVKShaderVisibility(set.GetShaderVisiblity());
            descriptorBindings[j].pImmutableSamplers = nullptr;
        }
        descriptorSetLayoutInfo[i].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorSetLayoutInfo[i].bindingCount = rangeCount;
        descriptorSetLayoutInfo[i].pBindings = descriptorBindings.data();
        descriptorSetLayoutInfo[i].pNext = nullptr;
    }

    //vkCreateDescriptorSetLayout(device->GetDevice(), )





    return true;
}

void VulkanDescriptorSetLayout::Destroy()
{

}