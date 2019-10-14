#ifndef __VKDEVICECONTEXT_H__
#define __VKDEVICECONTEXT_H__

#ifdef _USE_VULKAN

#include "IDeviceContext.h"
#include "../lib/vulkan_wrapper.h"

class Window;

class VkDeviceContext : public IDeviceContext
{
public:
    VkDeviceContext(){}
    virtual ~VkDeviceContext(){}

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
private:
    VkInstance instance;
    VkDevice device;
    VkSurfaceKHR surface;
    VkPhysicalDevice gpu;
    uint32 queueFamilyIndex;
};

#endif
#endif
