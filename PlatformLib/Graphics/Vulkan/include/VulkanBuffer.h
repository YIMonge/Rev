#ifndef __VULKANBUFFER_H__
#define __VULKANBUFFER_H__
#if _USE_VULKAN

#include "VulkanDevice.h"
#include "revGraphicsBuffer.h"
#include "revMath.h"
#include "VulkanCommandList.h"

class VulkanBuffer : public revGraphicsBuffer
{
public:
    VulkanBuffer() :
    mappedMemory(nullptr)
    {}
    virtual ~VulkanBuffer(){}

    virtual bool Create(revDevice* device, const revArray<revVector3>& data, USAGE usage = USAGE::STATIC);
    virtual bool Create(revDevice* device, const revArray<revVector4>& data, USAGE usage = USAGE::STATIC);
    virtual bool Create(revDevice* device, const revArray<float>& data, USAGE usage = USAGE::STATIC);
    virtual bool Create(revDevice* device, const float* data, uint32 sizeOfBytes, uint32 length, USAGE usage = USAGE::STATIC);
    virtual void Destroy();

    bool Update(const float* data, uint32 sizeOfCopyBytes, uint32 offset = 0);

    enum class BUFFER_TYPE
    {
        VERTEX,
        CONSTANT,
    };

protected:
    BUFFER_TYPE type;
private:
    bool MapMemoryTypeToIndex(uint32 typeBits, VkFlags mask, uint32* typeIndex);

    void* mappedMemory;
    VkDeviceMemory deviceMemory;
    VkMemoryRequirements memoryRequirements;
    VkMemoryAllocateInfo memoryAllocateInfo;



};

class VulkanVertexBuffer : public VulkanBuffer
{
public:
    VulkanVertexBuffer()
    {
        type = BUFFER_TYPE::VERTEX;
    }
    virtual ~VulkanVertexBuffer(){}
};

class VulkanConstantBuffer : public VulkanBuffer
{
public:
    VulkanConstantBuffer()
    {
        type = BUFFER_TYPE::CONSTANT;
    }
    virtual ~VulkanConstantBuffer(){}

};

#endif
#endif