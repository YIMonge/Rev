#ifdef _USE_VULKAN

#include "VulkanSampler.h"

bool VulkanSampler::Create(VulkanDevice* device, const revTexture& texture)
{
    this->device = device;
    const auto& samplerDesc = texture.GetSamplerDesc();
    // sampler setting
    VkSamplerCreateInfo samplerCreateInfo = {};
    samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCreateInfo.pNext = nullptr;
    samplerCreateInfo.magFilter = ConvertToVKFilter(samplerDesc.GetMagFilter());
    samplerCreateInfo.minFilter = ConvertToVKFilter(samplerDesc.GetMinFilter());
    samplerCreateInfo.mipmapMode = ConvertToVKMipFilterMode(samplerDesc.GetMipFilter());
    samplerCreateInfo.addressModeU = ConvertToVKTextureAddressMode(samplerDesc.GetAddressModeU());
    samplerCreateInfo.addressModeV = ConvertToVKTextureAddressMode(samplerDesc.GetAddressModeV());
    samplerCreateInfo.addressModeW = ConvertToVKTextureAddressMode(samplerDesc.GetAddressModeW());
    samplerCreateInfo.mipLodBias = samplerDesc.GetMipLodBias();
    samplerCreateInfo.maxAnisotropy = static_cast<f32>(samplerDesc.GetMaxAnisotropy());
    samplerCreateInfo.compareOp = ConverToVKComparisonFunc(samplerDesc.GetComparisonFunc());
    samplerCreateInfo.minLod = samplerDesc.GetMinLod();
    samplerCreateInfo.maxLod = samplerDesc.GetMaxLod();
    samplerCreateInfo.borderColor = ConvertToVKBorderColor(samplerDesc.GetBorderColor());
    samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
    
    VkResult result = vkCreateSampler(device->GetDevice(), &samplerCreateInfo, nullptr, &sampler);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    return true;
}

void VulkanSampler::Destroy()
{
    vkDestroySampler(device->GetDevice(), sampler, nullptr);
}

#endif
