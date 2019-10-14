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
private:
    VkInstance instance;
    VkDevice device;
    VkSurfaceKHR surface;
    VkPhysicalDevice gpu;
};

#endif
#endif
