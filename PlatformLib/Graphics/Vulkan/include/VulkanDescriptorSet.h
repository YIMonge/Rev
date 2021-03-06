#ifndef __VULKANDESCRIPTORSET_H__
#define __VULKANDESCRIPTORSET_H__
#ifdef _USE_VULKAN

#include "VulkanDevice.h"
#include "VulkanDescriptorSetLayout.h"
#include "revMaterial.h"
#include "VulkanDescriptorPool.h"

class VulkanDescriptorSet
{
public:
    VulkanDescriptorSet():
        device(nullptr),
        maxDescriptors(0),
        allocNum(0)
        {}
    virtual ~VulkanDescriptorSet(){}

    bool Create(revDevice* device, const VulkanDescriptorSetLayout& layout, uint32 numDescriptors, const VulkanDescriptorPool& descriptorPool);
    void Destroy();

    void Update();
    void Apply(VulkanCommandList& commandList, const VkPipelineLayout& pipelineLayout);

    const VkDescriptorSet& GetHandle() const { return descriptorSet; }

    class Chunk
    {
    friend class VulkanDescriptorSet;
    public:
        uint32 GetBindingSlot() const { return binding; }
		void UpdateResource(uint32 index, DESCRIPTOR_TYPE type, const VkDescriptorImageInfo* imageInfo, const VkDescriptorBufferInfo* bufferInfo, const VkBufferView* texelView)
		{
			writeDescriptorSets[index].descriptorType = ConvertToVKDescriptorType(type);
			writeDescriptorSets[index].pImageInfo = imageInfo;
			writeDescriptorSets[index].pBufferInfo = bufferInfo;
			writeDescriptorSets[index].pTexelBufferView = texelView;
		}

    private:
        Chunk(VkWriteDescriptorSet* writeDescriptorSets,  uint32 allocationNum, uint32 binding)
        {
            this->writeDescriptorSets = writeDescriptorSets;
            this->numOfDescriptors = allocationNum;
            this->binding = binding;
        }

    private:
        VkWriteDescriptorSet* writeDescriptorSets;
        uint32 numOfDescriptors;
        uint32 binding;
    };

    Chunk Allocation(uint32 allocationNum, uint32 binding)
    {
        if(allocNum + allocationNum >= maxDescriptors) {
            NATIVE_LOGE("Out of range descriptor num");
            return Chunk(nullptr, 0, 0);
        }

        VkWriteDescriptorSet writeDescriptorSet = {};
        writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSet.pNext = nullptr;
        writeDescriptorSet.dstSet = descriptorSet;
        writeDescriptorSet.dstBinding = binding;
        writeDescriptorSet.dstArrayElement = 0;
        writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_MAX_ENUM;
        writeDescriptorSet.descriptorCount = allocationNum;
        writeDescriptorSet.pImageInfo = nullptr;
        writeDescriptorSet.pBufferInfo = nullptr;
        writeDescriptorSet.pTexelBufferView = nullptr;

        writeDescriptorSets.push_back(writeDescriptorSet);
        Chunk ret(&writeDescriptorSets[writeDescriptorSets.size()-1], allocationNum, binding);
        allocNum += allocationNum;
        return ret;
    }

private:
    revDevice* device;
    uint32 maxDescriptors;
    revArray<VkWriteDescriptorSet> writeDescriptorSets;
    uint32 allocNum;
    VkDescriptorSet descriptorSet;
};

#endif
#endif
