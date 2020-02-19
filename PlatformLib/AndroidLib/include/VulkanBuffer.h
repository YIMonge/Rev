#ifndef __VULKANBUFFER_H__
#define __VULKANBUFFER_H__

#if _USE_VULKAN
#include "VulkanDeviceContext.h"
#include "IGraphicsBuffer.h"
#include "revMath.h"

class VulkanBuffer : public IGraphicsBuffer
{
public:
    VulkanBuffer(){}
    virtual ~VulkanBuffer(){}

    bool Create(const VulkanDeviceContext& deviceContext, const revArray<revVector3>& data, GRAPHICS_BUFFER_FORMAT format);
    bool Create(const VulkanDeviceContext& deviceContext, const revArray<revVector4>& data, GRAPHICS_BUFFER_FORMAT format);
    bool Create(const VulkanDeviceContext& deviceContext, const revArray<float>& data, GRAPHICS_BUFFER_FORMAT format);
    bool Create(const VulkanDeviceContext& deviceContext, const float* data, uint32 size, GRAPHICS_BUFFER_FORMAT format);
    void Destroy(const VulkanDeviceContext& deviceContext);

    virtual void Apply();

private:
    bool MapMemoryTypeToIndex(const VulkanDeviceContext& deviceContext , uint32 typeBits, VkFlags mask, uint32* typeIndex);


    VkBuffer buffer;

};

#endif
#endif
