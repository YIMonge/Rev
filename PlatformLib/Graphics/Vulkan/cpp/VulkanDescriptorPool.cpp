#ifdef _USE_VULKAN

#include "VulkanDescriptorPool.h"
#include "Log.h"

bool VulkanDescriptorPool::Create(revDevice* device, uint32 cbufferResourceMaxNum, uint32 textureResourceMaxNum)
{
    const uint32 MAX_HEAP_TYPE_NUM = static_cast<uint32>(DESCRIPTOR_HEAP_TYPE::MAX_NUM);
    VkDescriptorPoolSize descriptorPoolSize[MAX_HEAP_TYPE_NUM];
    descriptorPoolSize[static_cast<uint32>(DESCRIPTOR_HEAP_TYPE::BUFFER)].descriptorCount = cbufferResourceMaxNum;
    descriptorPoolSize[static_cast<uint32>(DESCRIPTOR_HEAP_TYPE::BUFFER)].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

    descriptorPoolSize[static_cast<uint32>(DESCRIPTOR_HEAP_TYPE::TEXTURE)].descriptorCount = textureResourceMaxNum;
    descriptorPoolSize[static_cast<uint32>(DESCRIPTOR_HEAP_TYPE::TEXTURE)].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

	descriptorPoolSize[static_cast<uint32>(DESCRIPTOR_HEAP_TYPE::SAMPLER)].descriptorCount = textureResourceMaxNum;
	descriptorPoolSize[static_cast<uint32>(DESCRIPTOR_HEAP_TYPE::SAMPLER)].type = VK_DESCRIPTOR_TYPE_SAMPLER;


    // TODO:
    descriptorPoolSize[3] = {};
    descriptorPoolSize[4] = {};

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
    descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolCreateInfo.pNext = nullptr;
    descriptorPoolCreateInfo.maxSets = 128;     // TODO: layout num
    descriptorPoolCreateInfo.poolSizeCount = 3;
    descriptorPoolCreateInfo.pPoolSizes = descriptorPoolSize;

    VkResult result = vkCreateDescriptorPool(device->GetDevice(), &descriptorPoolCreateInfo, nullptr, &descriptorPool);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    return true;
}



#endif
