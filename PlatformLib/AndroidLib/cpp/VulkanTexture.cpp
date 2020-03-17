#ifdef _USE_VULKAN
#include "VulkanTexture.h"
#include "File.h"
#include "Log.h"

#define STB_IMAGE_IMPLEMENTATION
// TODO: set memory allocation way
//#define STBI_MALLOC(sz)           malloc(sz)
//#define STBI_REALLOC(p,newsz)     realloc(p,newsz)
//#define STBI_FREE(p)              free(p)
#include "stb/stb_image.h"

VulkanTexture::VulkanTexture() :
imageLayout(VK_IMAGE_LAYOUT_GENERAL)
{
}

VulkanTexture::~VulkanTexture()
{
}

bool VulkanTexture::LoadFromFile(const VulkanDeviceContext &deviceContext, const char *path)
{
    const VkDevice& device = deviceContext.GetDevice();
    const VkFormat TEXTURE_FORMAT = VK_FORMAT_R8G8B8A8_UNORM;
    bool needBit = true;
    VkFormatProperties properties;
    vkGetPhysicalDeviceFormatProperties(deviceContext.GetGpuDevice(), TEXTURE_FORMAT, &properties);
    if(!((properties.linearTilingFeatures | properties.optimalTilingFeatures)& VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT)){
        return false;
    }
    if(properties.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) {
        needBit = false;
    }

    File file;
    if(!file.Open(path, FileMode::ReadBinary)){
        NATIVE_LOGE("file not found : %s", path);
        return false;
    }
    uint32 fileLength = file.GetFileSize();
    char* data = new char[fileLength];
    file.ReadData(data);
    file.Close();

    int n;
    uint8* imageData = stbi_load_from_memory(data, fileLength, reinterpret_cast<int32*>(&width), reinterpret_cast<int32*>(&height), &n, 4);

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
            .pQueueFamilyIndices = deviceContext.GetQueueFamilyIndexPtr(),
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

    if(!AllocateMemoryTypeFromProperties(deviceContext,
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
    stbi_image_free(imageData);
    delete[] data;

    //-----------------------------------------`-----------------------
    // sampler setting
    VkSamplerCreateInfo samplerCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .pNext = nullptr,
            .magFilter = VK_FILTER_NEAREST,
            .minFilter = VK_FILTER_NEAREST,
            .mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST,
            .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .mipLodBias = 0.0f,
            .maxAnisotropy = 1,
            .compareOp = VK_COMPARE_OP_NEVER,
            .minLod = 0.0f,
            .maxLod = 0.0f,
            .borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
            .unnormalizedCoordinates = VK_FALSE,
    };
    result = vkCreateSampler(device, &samplerCreateInfo, nullptr, &sampler);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    VkImageViewCreateInfo imageViewCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .image = VK_NULL_HANDLE,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = TEXTURE_FORMAT,
            .components =
                    {
                            VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G,
                            VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A,
                    },
            .subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1},
            .flags = 0,
    };
    imageViewCreateInfo.image = image;
    result = vkCreateImageView(device, &imageViewCreateInfo, nullptr, &resourceView);
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
