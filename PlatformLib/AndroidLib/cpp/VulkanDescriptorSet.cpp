#include "VulkanDescriptorSet.h"
#include "revArray.h"

// TODO: reference DX12 HEAP


bool VulkanDescriptorSet::Create(revDevice* device, DESCRIPTOR_HEAP_TYPE type, const VulkanDescriptorSetLayout& layout, uint32 numDescriptors, bool shaderVisiblity)
{
    this->device = device;
    this->type = type;
    this->maxDescriptors = numDescriptors;

    VkDescriptorPoolSize descriptorPoolSize = {};
    descriptorPoolSize.type = ConvertToVKDescriptorHeapType(type);
    descriptorPoolSize.descriptorCount = numDescriptors;

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
    descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolCreateInfo.pNext = nullptr;
    descriptorPoolCreateInfo.maxSets = 1;
    descriptorPoolCreateInfo.poolSizeCount = 1;
    descriptorPoolCreateInfo.pPoolSizes = &descriptorPoolSize;

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
    descriptorSetAllocateInfo.pSetLayouts = layout.GetDescriptorSet();
    result = vkAllocateDescriptorSets(device->GetDevice(), &descriptorSetAllocateInfo, &descriptorSet);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
}

void VulkanDescriptorSet::Destroy()
{
    vkDestroyDescriptorPool(device->GetDevice(), descriptorPool, nullptr);
}