#ifndef __VULKANBUFFER_H__
#define __VULKANBUFFER_H__

#if _USE_VULKAN

#include "VulkanDevice.h"
#include "revGraphicsBuffer.h"
#include "revMath.h"

class VulkanBuffer : public revGraphicsBuffer
{
public:
    VulkanBuffer(){}
    virtual ~VulkanBuffer(){}

    virtual bool Create(const revDevice& deviceContext, const revArray<revVector3>& data);
    virtual bool Create(const revDevice& deviceContext, const revArray<revVector4>& data);
    virtual bool Create(const revDevice& deviceContext, const revArray<float>& data);
    virtual bool Create(const revDevice& deviceContext, const float* data, uint32 size);
    virtual void Destroy(const revDevice& deviceContext);
private:
    bool MapMemoryTypeToIndex(const VulkanDevice& deviceContext , uint32 typeBits, VkFlags mask, uint32* typeIndex);
};

#endif
#endif
