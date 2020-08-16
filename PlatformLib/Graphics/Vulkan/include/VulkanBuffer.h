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
    VulkanBuffer(revDevice* device) :
		revGraphicsBuffer(device),
	    mappedMemory(nullptr)
    {}
    virtual ~VulkanBuffer(){}

    virtual bool Create(const void* data, uint32 sizeOfBytes, uint32 length, USAGE usage = USAGE::STATIC);
    virtual void Destroy();

    bool Update(const void* data, uint32 sizeOfCopyBytes, uint32 offset = 0);

protected:
	VkBufferUsageFlags bufferUsageFlags;

private:
	bool CreateBuffer(uint32 size, VkBufferUsageFlags usageFlag, VkMemoryPropertyFlags properties, revGraphicsResourceHandle& buffer, VkDeviceMemory& memory);

    void* mappedMemory;
    VkDeviceMemory deviceMemory; 
    VkMemoryAllocateInfo memoryAllocateInfo;
};

class VulkanVertexBuffer : public VulkanBuffer
{
public:
    VulkanVertexBuffer(revDevice* device) :
		VulkanBuffer(device)
    {
		bufferUsageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    }
    virtual ~VulkanVertexBuffer(){}
};

class VulkanConstantBuffer : public VulkanBuffer
{
public:
    VulkanConstantBuffer(revDevice* device) :
		VulkanBuffer(device)
    {
		bufferUsageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    }
    virtual ~VulkanConstantBuffer(){}

};

class VulkanIndexBuffer : public VulkanBuffer
{
public:
	VulkanIndexBuffer(revDevice* device) :
		VulkanBuffer(device)
	{
		bufferUsageFlags = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	}
	virtual ~VulkanIndexBuffer(){}
};

#endif
#endif
