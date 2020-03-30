#ifdef _USE_VULKAN
#include "VulkanTexture.h"
#include "File.h"

VulkanTexture::VulkanTexture() :
imageLayout(VK_IMAGE_LAYOUT_GENERAL)
{
}

VulkanTexture::~VulkanTexture()
{
}

bool VulkanTexture::CreateTexture(const revDeviceContext& deviceContext, uint8* imageData)
{
    const VkDevice& device = deviceContext.GetDevice();
    const VkFormat TEXTURE_FORMAT = VK_FORMAT_R8G8B8A8_UNORM;
    bool needBit = true;
    VkFormatProperties properties;
    // up cast...
    const VulkanDeviceContext vkDeviceContext = static_cast<const VulkanDeviceContext&>(deviceContext);
    vkGetPhysicalDeviceFormatProperties(vkDeviceContext.GetGpuDevice(), TEXTURE_FORMAT, &properties);
    if(!((properties.linearTilingFeatures | properties.optimalTilingFeatures)& VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT)){
        return false;
    }
    if(properties.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) {
        needBit = false;
    }

    VkImageCreateInfo imageCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext = nullptr,
            .imageType = VK_IMAGE_TYPE_2D,
            .format = TEXTURE_FORMAT,
            .extent = {width, height, 1},
            .mipLevels = 1,
            .arrayLayers = 1,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .tiling = VK_IMAGE_TILING_LINEAR,
            .usage = (needBit ? VK_IMAGE_USAGE_TRANSFER_SRC_BIT : VK_IMAGE_USAGE_SAMPLED_BIT),
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 1,
            .pQueueFamilyIndices = vkDeviceContext.GetQueueFamilyIndexPtr(),
            .initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED,
            .flags = 0,
    };
    VkMemoryAllocateInfo memoryAllocateInfo = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext = nullptr,
            .allocationSize = 0,
            .memoryTypeIndex = 0,
    };

    VkMemoryRequirements memoryRequirements;
    VkResult result = vkCreateImage(device, &imageCreateInfo, nullptr, &image);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    vkGetImageMemoryRequirements(device, image, &memoryRequirements);
    memoryAllocateInfo.allocationSize = memoryRequirements.size;

    if(!AllocateMemoryTypeFromProperties(vkDeviceContext,
            memoryRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
            &memoryAllocateInfo.memoryTypeIndex)){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    result = vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &deviceMemory);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    result = vkBindImageMemory(device, image, deviceMemory, 0);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    // TODO:need subresource?

    void* buffer;
    result = vkMapMemory(device, deviceMemory, 0, memoryAllocateInfo.allocationSize, 0, &buffer);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    memcpy(buffer, imageData, width * height * 4);
    vkUnmapMemory(device, deviceMemory);

    const SamplerDesc& samplerDesc = metaData.sampler;
    //-----------------------------------------`-----------------------
    // sampler setting
    VkSamplerCreateInfo samplerCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .pNext = nullptr,
            .magFilter = ConvertToVKFilter(samplerDesc.GetMagFilter()),
            .minFilter = ConvertToVKFilter(samplerDesc.GetMinFilter()),
            .mipmapMode = ConvertToVKMipFilterMode(samplerDesc.GetMipFilter()),
            .addressModeU = ConvertToVKTextureAddressMode(samplerDesc.GetAddressModeU()),
            .addressModeV = ConvertToVKTextureAddressMode(samplerDesc.GetAddressModeV()),
            .addressModeW = ConvertToVKTextureAddressMode(samplerDesc.GetAddressModeW()),
            .mipLodBias = samplerDesc.GetMipLodBias(),
            .maxAnisotropy = static_cast<f32>(samplerDesc.GetMaxAnisotropy()),
            .compareOp = ConverToVKComparisonFunc(samplerDesc.GetComparisonFunc()),
            .minLod = samplerDesc.GetMinLod(),
            .maxLod = samplerDesc.GetMaxLod(),
            .borderColor = ConvertToVKBorderColor(samplerDesc.GetBorderColor()),
            .unnormalizedCoordinates = VK_FALSE,
    };
    result = vkCreateSampler(device, &samplerCreateInfo, nullptr, &sampler);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    return true;
}

bool VulkanTexture::AllocateMemoryTypeFromProperties(const VulkanDeviceContext& deviceContext, uint32 typeBits, VkFlags requimentMask, uint32* typeIndex)
{
    const VkPhysicalDeviceMemoryProperties& deviceMemoryProperties = deviceContext.GetPhysicalDeviceMemoryProperties();
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

VkDescriptorImageInfo VulkanTexture::GetDescriptorImageInfo()
{
    VkDescriptorImageInfo descriptorImageInfo = {
        .sampler = sampler,
        .imageView = resourceView,
        .imageLayout = imageLayout,
    };
    return descriptorImageInfo;
}

#endif
