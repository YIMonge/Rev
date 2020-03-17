#ifndef __VULKANBUFFER_H__
#define __VULKANBUFFER_H__

#if _USE_VULKAN
#include "VulkanDeviceContext.h"
#include "revGraphicsBuffer.h"
#include "revMath.h"

class VulkanBuffer : public revGraphicsBuffer
{
public:
    VulkanBuffer(){}
    virtual ~VulkanBuffer(){}

    bool Create(const revDeviceContext& deviceContext, const revArray<revVector3>& data, REV_GRAPHICS_BUFFER_FOMAT_FLAG format);
    bool Create(const revDeviceContext& deviceContext, const revArray<revVector4>& data, REV_GRAPHICS_BUFFER_FOMAT_FLAG format);
    bool Create(const revDeviceContext& deviceContext, const revArray<float>& data, REV_GRAPHICS_BUFFER_FOMAT_FLAG format);
    bool Create(const revDeviceContext& deviceContext, const float* data, uint32 size, REV_GRAPHICS_BUFFER_FOMAT_FLAG format);
    void Destroy(const revDeviceContext& deviceContext);

    virtual void Apply();

private:
    bool MapMemoryTypeToIndex(const VulkanDeviceContext& deviceContext , uint32 typeBits, VkFlags mask, uint32* typeIndex);
};

#endif
#endif
