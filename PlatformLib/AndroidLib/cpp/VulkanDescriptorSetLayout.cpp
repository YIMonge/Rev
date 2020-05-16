#include "VulkanDescriptorSetLayout.h"

DESCRIPTOR_HEAP_TYPE DescriptorTypeToHeapType(DESCRIPTOR_TYPE type)
{
    DESCRIPTOR_HEAP_TYPE table[] = {
        DESCRIPTOR_HEAP_TYPE::RESOURCE,
        DESCRIPTOR_HEAP_TYPE::RESOURCE,
        DESCRIPTOR_HEAP_TYPE::RESOURCE,
        DESCRIPTOR_HEAP_TYPE::RESOURCE,

        DESCRIPTOR_HEAP_TYPE::RESOURCE,
        DESCRIPTOR_HEAP_TYPE::RESOURCE,
        DESCRIPTOR_HEAP_TYPE::RESOURCE,
        DESCRIPTOR_HEAP_TYPE::RESOURCE,

        DESCRIPTOR_HEAP_TYPE::RESOURCE,
        DESCRIPTOR_HEAP_TYPE::SAMPLER,
    };
    return table[static_cast<uint32>(type)];
}

bool VulkanDescriptorSetLayout::Create(revDevice* device, const revDescriptorBindingDesc& desc)
{
    this->device = device;
    uint32 setCount = desc.GetDescriptorSetLayoutCount();
    descriptorHeapTypes.reserve(setCount);
    descriptorSetLayouts.reserve(setCount);
    for(uint32 i = 0; i < setCount; ++i){
        auto& set = desc.GetDescriptorSetLayout(i);
        uint32 rangeCount = set.GetRangeCount();
        revArray<VkDescriptorSetLayoutBinding> descriptorBindings(rangeCount);
        for(uint32 j = 0; j < rangeCount; ++j){
            auto& range = set.GetRange(j);
            descriptorBindings[j].binding = range.registerIndex;
            descriptorBindings[j].descriptorType = ConvertToVKDescriptorType(set.GetDescriptorType());
            descriptorBindings[j].descriptorCount = range.count;
            descriptorBindings[j].stageFlags = ConvertToVKShaderVisibility(set.GetShaderVisiblity());
            descriptorBindings[j].pImmutableSamplers = nullptr;
        }
        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
        descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorSetLayoutCreateInfo.bindingCount = rangeCount;
        descriptorSetLayoutCreateInfo.pBindings = descriptorBindings.data();
        descriptorSetLayoutCreateInfo.pNext = nullptr;

        VkDescriptorSetLayout descriptorSetLayout;
        VkResult result = vkCreateDescriptorSetLayout(device->GetDevice(),
                &descriptorSetLayoutCreateInfo,
                nullptr,
                &descriptorSetLayout);
        if(result != VK_SUCCESS) {
            NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
            return false;
        }
        descriptorHeapTypes.push_back(DescriptorTypeToHeapType(set.GetDescriptorType()));
        descriptorSetLayouts.push_back(descriptorSetLayout);
    }
    return true;
}

void VulkanDescriptorSetLayout::Destroy()
{
    for (uint32 i = 0; i < descriptorSetLayouts.size(); ++i) {
        vkDestroyDescriptorSetLayout(device->GetDevice(), descriptorSetLayouts[i], nullptr);
    }
    descriptorSetLayouts.clear();
}
