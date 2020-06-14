#ifdef _USE_VULKAN

#include <revArray.h>
#include <revVector3.h>
#include "VulkanBuffer.h"
#include "Log.h"


bool VulkanBuffer::Create(revDevice* device, const revArray<revVector3>& data, USAGE usage)
{
    return Create(device, static_cast<const float*>(&(data[0].data[0])), sizeof(revVector3), static_cast<uint32>(data.size()));
}

bool VulkanBuffer::Create(revDevice* device, const revArray<revVector4>& data, USAGE usage)
{
    return Create(device, static_cast<const float*>(&(data[0].data[0])), sizeof(revVector4), static_cast<uint32>(data.size()));
}

bool VulkanBuffer::Create(revDevice* device, const revArray<float>& data, USAGE usage)
{
    return Create(device, static_cast<const float*>(&data[0]), sizeof(float), static_cast<uint32>(data.size()));
}

bool VulkanBuffer::Create(revDevice* device, const float* data, uint32 sizeOfBytes, uint32 length, USAGE usage)
{
    this->usage = usage;
    this->device = device;
    this->length = length;
    this->sizeOfBytes = sizeOfBytes;

    uint32 size = sizeOfBytes * length;

    VulkanDevice* vulkanDevice = (VulkanDevice*)(device);
    VkDevice revDevice = vulkanDevice->GetDevice();
    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.pNext = nullptr;
    bufferCreateInfo.size = size;
    bufferCreateInfo.usage = type == BUFFER_TYPE::VERTEX ? VK_BUFFER_USAGE_VERTEX_BUFFER_BIT : VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufferCreateInfo.flags = 0;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferCreateInfo.queueFamilyIndexCount = 1;
    bufferCreateInfo.pQueueFamilyIndices = vulkanDevice->GetQueueFamilyIndexPtr();

    VkResult result = vkCreateBuffer(revDevice, &bufferCreateInfo, nullptr, &buffer);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    // prepare for allocation
    vkGetBufferMemoryRequirements(revDevice, buffer, &memoryRequirements);

    memoryAllocateInfo = {};
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.pNext = nullptr;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = 0;
    
    if(!MapMemoryTypeToIndex(memoryRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            &memoryAllocateInfo.memoryTypeIndex)){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    // allocate memory
    result = vkAllocateMemory(revDevice, &memoryAllocateInfo, nullptr, &deviceMemory);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
        return false;
    }

    Update(data, size);

    result = vkBindBufferMemory(revDevice, buffer, deviceMemory, 0);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    return true;
}

bool VulkanBuffer::Update(const float* data, uint32 sizeOfCopyBytes, uint32 offset)
{
    // TODO:offset
    revGraphicsDevice revDevice = device->GetDevice();

    if(mappedMemory == nullptr) {
        VkResult result = vkMapMemory(revDevice, deviceMemory, 0, memoryAllocateInfo.allocationSize, 0, &mappedMemory);
        if (result != VK_SUCCESS) {
            NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
            return false;
        }
    }
    if (data != nullptr) {
        memcpy(mappedMemory, data, sizeOfCopyBytes);
    }
    if(usage == USAGE::STATIC) {
        vkUnmapMemory(revDevice, deviceMemory);
        mappedMemory = nullptr;
    }
    return true;
}

bool VulkanBuffer::MapMemoryTypeToIndex(uint32 typeBits, VkFlags mask, uint32* typeIndex)
{
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(device->GetAdapter(), &memoryProperties);
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

void VulkanBuffer::Destroy()
{
    vkDestroyBuffer(device->GetDevice(), buffer, nullptr);
}

#endif
