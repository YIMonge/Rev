#ifndef __VulkanDeviceContext_H__
#define __VulkanDeviceContext_H__

#ifdef _USE_VULKAN

#include "IDeviceContext.h"
#include "../lib/vulkan_wrapper.h"
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

    const VkDevice& GetDevice() const
    {
        return device;
    }

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

    const VkQueue& GetQueue() const
    {
        return queue;
    }

private:
    VkInstance instance;
    VkDevice device;
    VkSurfaceKHR surface;
    VkPhysicalDevice gpu;
    VkQueue queue;
    uint32 queueFamilyIndex;

#ifdef _DEBUG
    void initializeDebugLayer();
    revArray<const char*> debugLayers;
#endif
};

#endif
#endif
