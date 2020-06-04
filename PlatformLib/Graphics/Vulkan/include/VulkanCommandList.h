#ifndef __VULKANCOMMANDLIST_H__
#define __VULKANCOMMANDLIST_H__
#ifdef _USE_VULKAN

#include "revGraphicsCommandList.h"

class VulkanCommandList : public revGraphicsCommandList
{
public:
    VulkanCommandList() { commandAllocator = VK_NULL_HANDLE; }
    virtual ~VulkanCommandList(){}

    virtual bool Create(revDevice* device, revGraphicsPipeline* pipeline, revGraphicsCommandAllocator* allocator = nullptr);
    virtual void Destroy();
    virtual void Open();
    virtual void Close();

    void AddTransitionBarrier(const VkImage& resource, VkImageLayout before, VkImageLayout after, VkPipelineStageFlags srcStages, VkPipelineStageFlags destStages);
};

#endif
#endif