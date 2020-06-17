#ifndef __VULKANDESCRIPTORSET_H__
#define __VULKANDESCRIPTORSET_H__
#ifdef _USE_VULKAN

#include "VulkanDevice.h"
#include "VulkanDescriptorSetLayout.h"
#include "revMaterial.h"

class VulkanDescriptorSet
{
public:
    VulkanDescriptorSet():
        device(nullptr),
        maxDescriptors(0),
        allocNum(0)
        {}
    virtual ~VulkanDescriptorSet(){}

    bool Create(revDevice* device, const VulkanDescriptorSetLayout& layout, uint32 numDescriptors, bool shaderVisiblity);
    void Destroy();

    void Apply(VulkanCommandList& commandList, const VkPipelineLayout& pipelineLayout);

    const VkDescriptorSet& GetHandle() const { return descriptorSet; }

    class Chunk
    {
    friend class VulkanDescriptorSet;
    public:
        const VkDescriptorSet* GetDescriptorHandle() const { return descriptorSet; }
        uint32 GetNumOfDescription() const { return numOfDescriptors; }
        uint32 GetBindingSlot() const { return binding; }
    private:
        Chunk(VkDescriptorSet* descriptorSet, uint32 allocationNum, uint32 binding)
        {
            this->descriptorSet = descriptorSet;
            this->numOfDescriptors = allocationNum;
            this->binding = binding;
        }

    private:
        VkDescriptorSet* descriptorSet;
        uint32 numOfDescriptors;
        uint32 binding;
    };

    Chunk Allocation(uint32 allocationNum)
    {
        if(allocNum + allocationNum >= maxDescriptors) {
            NATIVE_LOGE("Out of range descriptor num");
            return Chunk(nullptr, 0, 0);
        }

        return Chunk(&descriptorSet, allocationNum, allocNum);
        // kokokara
    }

private:
    revDevice* device;
    uint32 maxDescriptors;
    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSet;
    uint32 allocNum;
};

#endif
#endif
