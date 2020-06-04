#ifdef _USE_VULKAN

#include "VulkanSampler.h"

bool VulkanSampler::Create(VulkanDevice* device, const revTexture& texture)
{
    this->device = device;
    const auto& samplerDesc = texture.GetSamplerDesc();
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
