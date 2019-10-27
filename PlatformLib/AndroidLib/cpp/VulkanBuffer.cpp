#include <revArray.h>
#include <revVector3.h>
#include "VulkanBuffer.h"

#if _USE_VULKAN


bool VulkanBuffer::Create(const VulkanDeviceContext& deviceContext, const revArray<revVector3>& data)
{
    return Create(deviceContext, static_cast<const float*>(&(data[0].data[0])), sizeof(revVector3) * data.Count());
}

bool VulkanBuffer::Create(const VulkanDeviceContext& deviceContext, const revArray<revVector4>& data)
{
    return Create(deviceContext, static_cast<const float*>(&(data[0].data[0])), sizeof(revVector4) * data.Count());
}

bool VulkanBuffer::Create(const VulkanDeviceContext& deviceContext, const revArray<float>& data)
{
    return Create(deviceContext, static_cast<const float*>(&data[0]), sizeof(float) * data.Count());
}

bool VulkanBuffer::Create(const VulkanDeviceContext& deviceContext, const float* data, uint32 size)
{
    VkDevice device = deviceContext.GetDevice();
    VkBufferCreateInfo bufferCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .size = size,
            .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,     // TODO: decide by param
            .flags = 0,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .pQueueFamilyIndices = deviceContext.GetQueueFamilyIndexPtr(),
            .queueFamilyIndexCount = 1,
    };

    VkResult result = vkCreateBuffer(device, &bufferCreateInfo, nullptr, &buffer);
    if(result != VK_SUCCESS) return false;

    // prepare for allocation
    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);

    VkMemoryAllocateInfo memoryAllocateInfo = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext = nullptr,
            .allocationSize = memoryRequirements.size,
            .memoryTypeIndex = 0,
    };
    if(MapMemoryTypeToIndex(deviceContext,
            memoryRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            &memoryAllocateInfo.memoryTypeIndex) == false){
        return false;
    }

    // allocate memory
    VkDeviceMemory deviceMemory;
    result = vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &deviceMemory);
    if(result != VK_SUCCESS) return false;

    void* tempBuffer;
    result = vkMapMemory(device, deviceMemory, 0, memoryAllocateInfo.allocationSize, 0, &tempBuffer);
    if(result != VK_SUCCESS) return false;
    memcpy(tempBuffer, data, size);
    vkUnmapMemory(device, deviceMemory);

    result = vkBindBufferMemory(device, buffer, deviceMemory, 0);
    if(result != VK_SUCCESS) return false;

    return true;
}

bool VulkanBuffer::MapMemoryTypeToIndex(const VulkanDeviceContext& deviceContext, uint32 typeBits, VkFlags mask, uint32* typeIndex)
{
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(deviceContext.GetGpuDevice(), &memoryProperties);
    for(uint32 i = 0; i < 32; ++i){
        if((typeBits & 1) == 1){
            if((memoryProperties.memoryTypes[i].propertyFlags & mask) == mask){
                *typeIndex = i;
                return true;
            }
        }
        typeBits >>= 1;
    }
    return false;
}

void VulkanBuffer::Destroy(const VulkanDeviceContext& deviceContext)
{
    vkDestroyBuffer(deviceContext.GetDevice(), buffer, nullptr);
}

#endif
