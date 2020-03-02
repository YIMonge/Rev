#ifdef _USE_VULKAN
#include "VulkanTexture.h"
#include "File.h"
#include "Log.h"

VulkanTexture::VulkanTexture()
{
}

VulkanTexture::~VulkanTexture()
{
}

bool VulkanTexture::LoadFromFile(const VulkanDeviceContext &deviceContext, const char *path)
{
    bool needBit = true;
    VkFormatProperties properties;
    vkGetPhysicalDeviceFormatProperties(deviceContext.GetGpuDevice(), VK_FORMAT_R8G8B8A8_UNORM, &properties);
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
    uint8* data = new uint8[fileLength];
    file.ReadData(data);
    file.Close();

    // TODO:stbi_load_from_memory




    delete[] data;
}

#endif
