#ifdef _USE_VULKAN

#include "VulkanDescriptorSet.h"
#include "revArray.h"

// TODO: reference DX12 HEAP


bool VulkanDescriptorSet::Create(revDevice* device, const VulkanDescriptorSetLayout& layout, uint32 numDescriptors, const VulkanDescriptorPool& descriptorPool)
{
    this->device = device;
    this->maxDescriptors = numDescriptors;
    this->writeDescriptorSets.reserve(numDescriptors);

    VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
    descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorSetAllocateInfo.pNext = nullptr;
    descriptorSetAllocateInfo.descriptorPool = descriptorPool.GetHandle();
    descriptorSetAllocateInfo.descriptorSetCount = layout.GetDescriptorSetCount();
    descriptorSetAllocateInfo.pSetLayouts = layout.GetDescriptorSets();

    VkResult result = vkAllocateDescriptorSets(device->GetDevice(), &descriptorSetAllocateInfo, &descriptorSet);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    return true;
}

void VulkanDescriptorSet::Update()
{
    vkUpdateDescriptorSets(device->GetDevice(),
                           allocNum,                               // descriptorWriteCount
                           writeDescriptorSets.data(),             // pDescriptorWrites
                           0,                                      // descriptorCopyCount
                           nullptr);                               // pDescriptorCopies

    writeDescriptorSets.clear();
}

void VulkanDescriptorSet::Destroy()
{
    //vkDestroyDescriptorPool(device->GetDevice(), descriptorPool, nullptr);
}

void VulkanDescriptorSet::Apply(VulkanCommandList& commandList, const VkPipelineLayout& pipelineLayout)
{
    vkCmdBindDescriptorSets(commandList.GetList(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout,
            // TODO: memory chunk
            0,      // firstSet,
            1,      // descriptorSetCount,
            &descriptorSet,
            0,      // dynamic offset count
            nullptr // dynamic offsets (uint32*)
            );
}

#endif
