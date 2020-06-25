#ifdef _USE_VULKAN
#include "VulkanTextureView.h"

void VulkanTextureView::Create(revDevice* device, const revTexture& texture, const VulkanSampler& sampler, VulkanDescriptorSet::Chunk& chunk)
{
    this->device = device;

    VkImageViewCreateInfo imageViewCreateInfo = {};
    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCreateInfo.pNext = nullptr;
    imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCreateInfo.components =
    {
            VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G,
            VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A,
    };
    imageViewCreateInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
    imageViewCreateInfo.flags = 0;
    imageViewCreateInfo.format = ConvertToVKFormat(texture.GetFormat());
    imageViewCreateInfo.image = texture.GetHandle();

    VkResult result = vkCreateImageView(device->GetDevice(), &imageViewCreateInfo, nullptr, &resourceView);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return;
    }

    descriptorImageInfo.sampler = sampler.GetHandle();
    descriptorImageInfo.imageView = resourceView;
    descriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;


    VkWriteDescriptorSet* writeDescriptorSet = chunk.GetVkWriteDescriptorSet();
    for(uint32 i = 0; i < chunk.GetNumOfDescriptors(); ++i){
        writeDescriptorSet[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writeDescriptorSet[i].pImageInfo = &descriptorImageInfo;
    }
}

void VulkanTextureView::Destroy()
{
    vkDestroyImageView(device->GetDevice(), resourceView, nullptr);
}
#endif
