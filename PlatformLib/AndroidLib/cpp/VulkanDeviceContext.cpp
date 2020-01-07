#include "VulkanDeviceContext.h"
#include "Window.h"
#include "Log.h"

#ifdef _USE_VULKAN

bool VulkanDeviceContext::Create(Window& window)
{
    if(!InitVulkan())
    {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    VkApplicationInfo appInfo = {
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pNext = nullptr,
      .apiVersion = VK_MAKE_VERSION(1, 0, 0),
      .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
      .engineVersion = VK_MAKE_VERSION(1, 0, 0),
      .pApplicationName = "RevSample",
      .pEngineName = "Rev",
  };

    const int numOfInstanceExt = 2;
    const char* useInstanceExt[numOfInstanceExt] = 
    {
        "VK_KHR_surface",
        "VK_KHR_android_surface",
    };

    VkInstanceCreateInfo instanceInfo = {
        .sType                 = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, 
        .pNext                 = nullptr,
        .pApplicationInfo      = &appInfo,
        .enabledLayerCount     = 0,
        .ppEnabledLayerNames   = NULL,
        .enabledExtensionCount = numOfInstanceExt,      
        .ppEnabledExtensionNames = useInstanceExt,      
    };
    VkResult result = vkCreateInstance(&instanceInfo, nullptr, &instance);
    if(result != VK_SUCCESS){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    VkAndroidSurfaceCreateInfoKHR surfaceInfo{
        .sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .window = window.GetHandle(),
    };
    result = vkCreateAndroidSurfaceKHR(instance, &surfaceInfo, nullptr, &surface);
    if(result != VK_SUCCESS){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    uint32 gpuCount = 0;
    result = vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr);
    if(result != VK_SUCCESS){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    VkPhysicalDevice tmpGpus[gpuCount];
    result = vkEnumeratePhysicalDevices(instance, &gpuCount, tmpGpus);
    if(result != VK_SUCCESS){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    gpu = tmpGpus[0];

    VkPhysicalDeviceProperties gpuProperties;
    vkGetPhysicalDeviceProperties(gpu, &gpuProperties);

    VkSurfaceCapabilitiesKHR surfaceCapablities;  
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &surfaceCapablities);

    uint32 queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueFamilyCount, nullptr);
    VkQueueFamilyProperties queueFamilyProperties[queueFamilyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueFamilyCount, queueFamilyProperties);


    // Find queue for graphics
    for (queueFamilyIndex=0; queueFamilyIndex < queueFamilyCount; queueFamilyIndex++) {
        if (queueFamilyProperties[queueFamilyIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            break;
        }
    }

    // for pQueuePriorities, it's an optional pointer but if it's null Vulkan will be crashed when creating device
    // by Vulkan Programing Guide, if set to null, priorities are same.
    // priorities must be normalized number such as 0.0 ~ 1.0.
    // higher priority will be allocated more processing resources and scheduled more aggressively.
    float priorities[] = {
        1.0f,
    };
    VkDeviceQueueCreateInfo queueCreateInfo{
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .pNext = nullptr,
      .flags = 0,
      .queueCount = 1,
      .queueFamilyIndex = queueFamilyIndex,
      .pQueuePriorities = priorities,
    };

    const uint32 numOfDeviceExt = 1;
    const char* useDeviceExt[numOfDeviceExt] = 
    {
        "VK_KHR_swapchain",
    };

    VkDeviceCreateInfo deviceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &queueCreateInfo,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = numOfDeviceExt,
        .ppEnabledExtensionNames = useDeviceExt,
        .pEnabledFeatures = nullptr,
    };
    result = vkCreateDevice(gpu, &deviceCreateInfo, nullptr, &device);
    if(result != VK_SUCCESS){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    return true;
}


void VulkanDeviceContext::Destroy()
{
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(instance, nullptr);
}

#endif