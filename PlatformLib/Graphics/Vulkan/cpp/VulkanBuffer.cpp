#ifdef _USE_VULKAN

#include <revArray.h>
#include <revVector3.h>
#include "VulkanBuffer.h"
#include "Log.h"

bool VulkanBuffer::CreateBuffer(uint32 size, VkBufferUsageFlags usageFlag, VkMemoryPropertyFlags properties, revGraphicsResource& buffer, VkDeviceMemory& memory)
{
	const auto& device = this->device->GetDevice();
	
	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.pNext = nullptr;
	bufferCreateInfo.size = size;
	bufferCreateInfo.usage = usageFlag;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkResult result = vkCreateBuffer(device, &bufferCreateInfo, nullptr, &buffer);
	if (result != VK_SUCCESS) {
		NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
		return false;
	}

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);

	memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	if (!MapMemoryTypeToIndex(this->device->GetAdapter(),
		memoryRequirements.memoryTypeBits,
		properties,
		&memoryAllocateInfo.memoryTypeIndex)) {
		NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
		return false;
	}

	result = vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &memory);
	if (result != VK_SUCCESS) {
		NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
		return false;
	}

	result = vkBindBufferMemory(device, buffer, memory, 0);
	if (result != VK_SUCCESS) {
		NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
		return false;
	}


	return true;
}


bool VulkanBuffer::Create(const void* data, uint32 sizeOfBytes, uint32 length, USAGE usage)
{
    this->usage = usage;
    this->length = length;
    this->sizeOfBytes = sizeOfBytes;

    uint32 size = sizeOfBytes * length;
	VkBufferUsageFlags usageFlags = bufferUsageFlags;
	VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	if (usage == USAGE::STATIC) {
		usageFlags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	}
	CreateBuffer(size, usageFlags, properties, buffer, deviceMemory);
	Update(data, size);

    return true;
}

bool VulkanBuffer::Update(const void* data, uint32 sizeOfCopyBytes, uint32 offset)
{
    revGraphicsDevice device = this->device->GetDevice();

	if (usage == USAGE::DYNAMIC) {
		if (mappedMemory == nullptr) {
			VkResult result = vkMapMemory(device, deviceMemory, 0, memoryAllocateInfo.allocationSize, 0, &mappedMemory);
			if (result != VK_SUCCESS) {
				NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
				return false;
			}
		}
		if (data != nullptr) {
			memcpy(mappedMemory, data, sizeOfCopyBytes);
		}
		vkUnmapMemory(device, deviceMemory);
		mappedMemory = nullptr;
    }
	else {
		VkDeviceMemory stagingMemory;
		VkBuffer stagingBuffer;
		CreateBuffer(sizeOfCopyBytes, bufferUsageFlags | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingMemory);
		VkResult result = vkMapMemory(device, stagingMemory, 0, sizeOfCopyBytes, 0, &mappedMemory);
		if (result != VK_SUCCESS) {
			NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
			return false;
		}
		memcpy(mappedMemory, data, sizeOfCopyBytes);
		vkUnmapMemory(device, stagingMemory);

		VulkanDevice* vulkanDevice = static_cast<VulkanDevice*>(this->device);
		vulkanDevice->GetGlobalCommandList().ReserveReleaseBuffer(stagingBuffer, stagingMemory);


		VkBufferCopy copyRegion = {};
		copyRegion.size = sizeOfCopyBytes;
		vkCmdCopyBuffer(vulkanDevice->GetGlobalCommandList().GetList(), stagingBuffer, buffer, 1, &copyRegion);
	}
    return true;
}

void VulkanBuffer::Destroy()
{
    vkDestroyBuffer(device->GetDevice(), buffer, nullptr);
}

#endif
