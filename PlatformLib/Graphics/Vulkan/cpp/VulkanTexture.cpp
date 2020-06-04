#ifdef _USE_VULKAN
#include "VulkanTexture.h"
#include "File.h"

VulkanTexture::VulkanTexture() :
        imageLayout(VK_IMAGE_LAYOUT_UNDEFINED)
{
}

VulkanTexture::~VulkanTexture()
{
}

bool VulkanTexture::CreateTexture(revDevice* device, uint8* imageData)
{
    const VkDevice& revDevice = device->GetDevice();
    const VkFormat TEXTURE_FORMAT = ConvertToVKFormat(GetFormat());
    bool needBlit = true;
    VkFormatProperties properties;
    vkGetPhysicalDeviceFormatProperties(device->GetAdapter(), TEXTURE_FORMAT, &properties);
    if(!((properties.linearTilingFeatures | properties.optimalTilingFeatures) & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT)){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    if(properties.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) {
        needBlit = false;
    }

    VkImageCreateInfo imageCreateInfo = {};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.pNext = nullptr;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = TEXTURE_FORMAT;
    imageCreateInfo.extent = { width, height, 1 };
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
    imageCreateInfo.usage = (needBlit ? VK_IMAGE_USAGE_TRANSFER_SRC_BIT : VK_IMAGE_USAGE_SAMPLED_BIT);
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.queueFamilyIndexCount = 1;
    imageCreateInfo.pQueueFamilyIndices = static_cast<VulkanDevice*>(device)->GetQueueFamilyIndexPtr();
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
    imageCreateInfo.flags = 0;

    VkMemoryAllocateInfo memoryAllocateInfo = {};
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.pNext = nullptr;
    memoryAllocateInfo.allocationSize = 0;
    memoryAllocateInfo.memoryTypeIndex = 0;

    VkMemoryRequirements memoryRequirements;
    VkResult result = vkCreateImage(revDevice, &imageCreateInfo, nullptr, &handle);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    vkGetImageMemoryRequirements(revDevice, handle, &memoryRequirements);
    memoryAllocateInfo.allocationSize = memoryRequirements.size;

    if(!AllocateMemoryTypeFromProperties(static_cast<VulkanDevice*>(device),
            memoryRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
            &memoryAllocateInfo.memoryTypeIndex)){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    result = vkAllocateMemory(revDevice, &memoryAllocateInfo, nullptr, &deviceMemory);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    result = vkBindImageMemory(revDevice, handle, deviceMemory, 0);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    // TODO: VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT check
    VkImageSubresource imageSubresource = {};
    imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageSubresource.mipLevel = 0;
    imageSubresource.arrayLayer = 0;

    VkSubresourceLayout subresourceLayout;
    vkGetImageSubresourceLayout(revDevice, handle, &imageSubresource, &subresourceLayout);

    void* buffer;
    result = vkMapMemory(revDevice, deviceMemory, 0, memoryAllocateInfo.allocationSize, 0,
                         &buffer);
    if (result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__, __LINE__);
        return false;
    }
    memcpy(buffer, imageData, width * height * 4);
    vkUnmapMemory(revDevice, deviceMemory);

    imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    //TODO: move AddTransitionBarrier to base class.
    VulkanDevice* vulkanDevice = static_cast<VulkanDevice*>(device);
    if(!needBlit){
        vulkanDevice->GetGlobalCommandList().AddTransitionBarrier(handle,
                                                                  VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                                                  VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);
    }
    else {
        NATIVE_LOGE("TODO: Create texture with need blit");
    }

    return true;
}

bool VulkanTexture::AllocateMemoryTypeFromProperties(VulkanDevice* device, uint32 typeBits, VkFlags requimentMask, uint32* typeIndex)
{
    const VkPhysicalDeviceMemoryProperties& deviceMemoryProperties = device->GetPhysicalDeviceMemoryProperties();
    for(uint32 i = 0; i < 32; ++i){
        if((typeBits & 1) == 1){
            if((deviceMemoryProperties.memoryTypes[i].propertyFlags & requimentMask) == requimentMask){
                *typeIndex = i;
                return true;
            }
        }
        typeBits >>= 1;
    }
    return false;
}
#endif
