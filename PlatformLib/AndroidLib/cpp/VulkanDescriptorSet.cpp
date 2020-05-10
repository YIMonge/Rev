#include "VulkanDescriptorSet.h"
#include "revArray.h"

// TODO: reference DX12 HEAP


bool VulkanDescriptorSet::Create(revDevice* device, const VulkanDescriptorSetLayout& layout, uint32 numDescriptors, bool shaderVisiblity)
{
    this->device = device;
    this->maxDescriptors = numDescriptors;

    uint32 descriptorSetCount =  layout.GetDescriptorSetCount();
    revArray<VkDescriptorPoolSize> descriptorPoolSizeArray(descriptorSetCount);
    for(uint32 i = 0; i < descriptorSetCount; ++i) {
        descriptorPoolSizeArray[i].type = ConvertToVKDescriptorHeapType(layout.GetDescriptorHeapType(i));
        descriptorPoolSizeArray[i].descriptorCount = numDescriptors;
    }

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
    descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolCreateInfo.pNext = nullptr;
    descriptorPoolCreateInfo.maxSets = descriptorSetCount;
    descriptorPoolCreateInfo.poolSizeCount = descriptorPoolSizeArray.size();
    descriptorPoolCreateInfo.pPoolSizes = descriptorPoolSizeArray.data();

    VkResult result = vkCreateDescriptorPool(device->GetDevice(), &descriptorPoolCreateInfo, nullptr, &descriptorPool);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
    descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorSetAllocateInfo.pNext = nullptr;
    descriptorSetAllocateInfo.descriptorPool = descriptorPool;
    descriptorSetAllocateInfo.descriptorSetCount = layout.GetDescriptorSetCount();
    descriptorSetAllocateInfo.pSetLayouts = layout.GetDescriptorSets();

    result = vkAllocateDescriptorSets(device->GetDevice(), &descriptorSetAllocateInfo, &descriptorSet);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    return true;
}

void VulkanDescriptorSet::Destroy()
{
    vkDestroyDescriptorPool(device->GetDevice(), descriptorPool, nullptr);
}