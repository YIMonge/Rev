#ifndef __VULKANBUFFER_H__
#define __VULKANBUFFER_H__

#if _USE_VULKAN

#include "VulkanDeviceContext.h"

class VulkanBuffer
{
public:
    VulkanBuffer(){}
    ~VulkanBuffer(){}

    bool Create(const VulkanDeviceContext& deviceContext, const revArray<revVector3>& data);
    bool Create(const VulkanDeviceContext& deviceContext, const revArray<revVector4>& data);
    bool Create(const VulkanDeviceContext& deviceContext, const revArray<float>& data);
    bool Create(const VulkanDeviceContext& deviceContext, const float* data, uint32 size);
    void Destroy(const VulkanDeviceContext& deviceContext);
private:
    bool MapMemoryTypeToIndex(const VulkanDeviceContext& deviceContext , uint32 typeBits, VkFlags mask, uint32* typeIndex);


    VkBuffer buffer;

};

#endif
#endif
