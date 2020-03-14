#ifndef __VulkanDeviceContext_H__
#define __VulkanDeviceContext_H__

#ifdef _USE_VULKAN

#include "IDeviceContext.h"
#ifdef _DEBUG
#include "revArray.h"
#endif

class Window;

class VulkanDeviceContext : public IDeviceContext
{
public:
    VulkanDeviceContext(){}
    virtual ~VulkanDeviceContext(){}

    bool Create(Window& window);
    void Destroy();



    const VkPhysicalDevice& GetGpuDevice() const
    {
        return gpu;
    }

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
    VkPhysicalDevice gpu;
    uint32 queueFamilyIndex;
    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;

#ifdef _DEBUG
    void initializeDebugLayer();
    revArray<const char*> debugLayers;
#endif
};

#endif
#endif
