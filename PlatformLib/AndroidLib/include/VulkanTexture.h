#ifndef __VULKANTEXTURE_H__
#define __VULKANTEXTURE_H__

#ifdef _USE_VULKAN

#include "VulkanDevice.h"
#include "revTexture.h"

class VulkanTexture : public revTexture
{
public:
    VulkanTexture();
    virtual ~VulkanTexture();

    VkDescriptorImageInfo GetDescriptorImageInfo();

protected:
    virtual bool CreateTexture(revDevice* device, uint8* imageData);

private:
    bool AllocateMemoryTypeFromProperties(VulkanDevice* device, uint32 typeBits, VkFlags requimentMask, uint32* typeIndex);
    VkDeviceMemory deviceMemory;
    VkImageLayout imageLayout;
};

#endif
#endif

