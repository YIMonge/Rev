#ifndef __VULKANDEVICE_H__
#define __VULKANDEVICE_H__

#ifdef _USE_VULKAN

#include "revDevice.h"
#ifdef _DEBUG
#include "revArray.h"
#endif

class Window;

class VulkanDevice : public revDevice
{
public:
    VulkanDevice(){}
    virtual ~VulkanDevice(){}

    bool Create(Window& window);
    void Destroy();

    const VkSurfaceKHR& GetSurface() const
    {
        return surface;
    }

    uint32 GetQueueFamilyIndex() const
    {
        return queueFamilyIndex;
    }

    const uint32* GetQueueFamilyIndexPtr() const
    {
        return &queueFamilyIndex;
    }

    const VkPhysicalDeviceMemoryProperties& GetPhysicalDeviceMemoryProperties() const {
        return physicalDeviceMemoryProperties;
    }
private:
    VkInstance instance;
    VkSurfaceKHR surface;
    uint32 queueFamilyIndex;
    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;

#ifdef _DEBUG
    void initializeDebugLayer();
    revArray<const char*> debugLayers;
#endif
};

#endif
#endif
