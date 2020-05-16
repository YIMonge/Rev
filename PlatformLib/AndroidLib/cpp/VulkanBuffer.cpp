#include <revArray.h>
#include <revVector3.h>
#include "VulkanBuffer.h"
#include "Log.h"

#if _USE_VULKAN

bool VulkanBuffer::Create(const revDevice& device, const revArray<revVector3>& data)
{
    return Create(device, static_cast<const float*>(&(data[0].data[0])), sizeof(revVector3), data.size());
}

bool VulkanBuffer::Create(const revDevice& device, const revArray<revVector4>& data)
{
    return Create(device, static_cast<const float*>(&(data[0].data[0])), sizeof(revVector4), data.size());
}

bool VulkanBuffer::Create(const revDevice& device, const revArray<float>& data)
{
    return Create(device, static_cast<const float*>(&data[0]), sizeof(float), data.size());
}

bool VulkanBuffer::Create(const revDevice& device, const float* data, uint32 sizeOfBytes, uint32 length)
{
    this->length = length;
    uint32 size = sizeOfBytes * length;

    const VulkanDevice vulkanDevice = static_cast<const VulkanDevice&>(device);
    VkDevice revDevice = vulkanDevice.GetDevice();
    VkBufferCreateInfo bufferCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .size = size,
            .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,     // TODO: decide by param(= format)
            .flags = 0,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .pQueueFamilyIndices = vulkanDevice.GetQueueFamilyIndexPtr(),
            .queueFamilyIndexCount = 1,
    };

    VkResult result = vkCreateBuffer(revDevice, &bufferCreateInfo, nullptr, &buffer);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    // prepare for allocation
    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(revDevice, buffer, &memoryRequirements);

    VkMemoryAllocateInfo memoryAllocateInfo = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext = nullptr,
            .allocationSize = memoryRequirements.size,
            .memoryTypeIndex = 0,
    };
    if(!MapMemoryTypeToIndex(vulkanDevice,
            memoryRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            &memoryAllocateInfo.memoryTypeIndex)){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    // allocate memory
    VkDeviceMemory deviceMemory;
    result = vkAllocateMemory(revDevice, &memoryAllocateInfo, nullptr, &deviceMemory);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
        return false;
    }

    void* tempBuffer;
    result = vkMapMemory(revDevice, deviceMemory, 0, memoryAllocateInfo.allocationSize, 0, &tempBuffer);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    memcpy(tempBuffer, data, size);
    vkUnmapMemory(revDevice, deviceMemory);

    result = vkBindBufferMemory(revDevice, buffer, deviceMemory, 0);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    return true;
}

bool VulkanBuffer::MapMemoryTypeToIndex(const VulkanDevice& deviceContext, uint32 typeBits, VkFlags mask, uint32* typeIndex)
{
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(deviceContext.GetAdapter(), &memoryProperties);
    for(uint32 i = 0; i < 32; ++i){
        if((typeBits & 1) == 1){
            if((memoryProperties.memoryTypes[i].propertyFlags & mask) == mask){
                *typeIndex = i;
                return true;
            }
        }
        typeBits >>= 1;
    }
    NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
    return false;
}

void VulkanBuffer::Destroy(const revDevice& deviceContext)
{
    vkDestroyBuffer(deviceContext.GetDevice(), buffer, nullptr);
}

void VulkanBuffer::Apply(VulkanCommandList& commandList)
{
    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers(commandList.GetList(), 0, 1, &buffer, &offset);
}


#endif
