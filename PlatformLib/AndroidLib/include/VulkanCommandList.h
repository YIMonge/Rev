#ifndef __VULKANCOMMANDLIST_H__
#define __VULKANCOMMANDLIST_H__

#include "revGraphicsCommandList.h"

class VulkanCommandList : public revGraphicsCommandList
{
public:
    VulkanCommandList() { commandAllocator = VK_NULL_HANDLE; }
    virtual ~VulkanCommandList(){}

    virtual bool Create(revDevice* device, revGraphicsPipeline* pipeline, revGraphicsCommandAllocator* allocator = nullptr);
    virtual void Open();
    virtual void Close();
};

#endif
