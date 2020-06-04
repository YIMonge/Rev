#ifdef _USE_VULKAN

#include "VulkanDevice.h"
#include "Window.h"
#include "Log.h"
#ifdef _DEBUG
#include <string.h>
#endif

bool VulkanDevice::Create(Window& window)
{
    if(!InitVulkan())
    {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
#ifdef _DEBUG
    initializeDebugLayer();
#endif
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pApplicationName = "RevSample";
	appInfo.pEngineName = "Rev";

    const int numOfInstanceExt = 2;
    const char* useInstanceExt[numOfInstanceExt] = 
    {
        "VK_KHR_surface",
        "VK_KHR_android_surface",
    };

    VkInstanceCreateInfo instanceInfo = {};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pNext = nullptr;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledExtensionCount = numOfInstanceExt;
    instanceInfo.ppEnabledExtensionNames = useInstanceExt;
   
#ifdef _DEBUG
    instanceInfo.enabledLayerCount     = debugLayers.size();
    instanceInfo.ppEnabledLayerNames   = &debugLayers[0];
#else
    instanceInfo.enabledLayerCount     = 0;
    instanceInfo.ppEnabledLayerNames   = nullptr;
#endif

    VkResult result = vkCreateInstance(&instanceInfo, nullptr, &instance);
    if(result != VK_SUCCESS){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

#ifdef _ANDROID
    VkAndroidSurfaceCreateInfoKHR surfaceInfo = {};
#elif defined(_WINDWOS)
    VkDisplaySurfaceCreateInfoKHR surfaceInfo = {};
#endif
    surfaceInfo.sType = VK_STRUCTURE_TYPE_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.pNext = nullptr;
	surfaceInfo.flags = 0;
	surfaceInfo.window = window.GetHandle();
    

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

    revArray<VkPhysicalDevice> tmpGpus(gpuCount);
    result = vkEnumeratePhysicalDevices(instance, &gpuCount, tmpGpus.data());
    if(result != VK_SUCCESS){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    adapter = tmpGpus[0];

    uint32 queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(adapter, &queueFamilyCount, nullptr);
    revArray<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(adapter, &queueFamilyCount, queueFamilyProperties.data());


    // Find queue for graphics
    for (queueFamilyIndex=0; queueFamilyIndex < queueFamilyCount; queueFamilyIndex++) {
        if (queueFamilyProperties[queueFamilyIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            break;
        }
    }

    if(queueFamilyIndex >= queueFamilyCount){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }


    // for pQueuePriorities, it's an optional pointer but if it's null Vulkan will be crashed when creating device
    // by Vulkan Programing Guide, if set to null, priorities are same.
    // priorities must be normalized number such as 0.0 ~ 1.0.
    // higher priority will be allocated more processing resources and scheduled more aggressively.
    float priorities[] = {
        1.0f,
    };
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.pNext = nullptr;
    queueCreateInfo.flags = 0;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
    queueCreateInfo.pQueuePriorities = priorities;
   

    const uint32 numOfDeviceExt = 1;
    const char* useDeviceExt[numOfDeviceExt] = 
    {
        "VK_KHR_swapchain",
    };

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = nullptr;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.enabledLayerCount = 0;
    deviceCreateInfo.ppEnabledLayerNames = nullptr;
    deviceCreateInfo.enabledExtensionCount = numOfDeviceExt;
    deviceCreateInfo.ppEnabledExtensionNames = useDeviceExt;
    deviceCreateInfo.pEnabledFeatures = nullptr;
  
    result = vkCreateDevice(adapter, &deviceCreateInfo, nullptr, &device);
    if(result != VK_SUCCESS){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    vkGetPhysicalDeviceMemoryProperties(adapter, &physicalDeviceMemoryProperties);
    vkGetDeviceQueue(device, queueFamilyIndex, 0, &queue);

    return true;
}

bool VulkanDevice::CreateCommandList(uint32 num)
{
    globalCommandList.Create(this, nullptr);

    commandLists.resize(num);
    for(uint32 i = 0; i < num; ++i){
        if(!commandLists[i].Create(this, nullptr)){
            return false;
        }
    }
    return true;
}

void VulkanDevice::Destroy()
{
    for(auto l : commandLists){
        l.Destroy();
    }
    globalCommandList.Destroy();
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(instance, nullptr);
}

#ifdef _DEBUG
void VulkanDevice::initializeDebugLayer()
{
    uint32 layerPropertyCount = 0;
    vkEnumerateInstanceLayerProperties(&layerPropertyCount, nullptr);
    revArray<VkLayerProperties> props(layerPropertyCount);
    vkEnumerateInstanceLayerProperties(&layerPropertyCount, props.data());

    // for NDK r20
    debugLayers.resize(5);
    debugLayers[0] = "VK_LAYER_GOOGLE_threading";
    debugLayers[1] = "VK_LAYER_LUNARG_parameter_validation";
    debugLayers[2] = "VK_LAYER_LUNARG_object_tracker";
    debugLayers[3] = "VK_LAYER_LUNARG_core_validation";
    debugLayers[4] = "VK_LAYER_GOOGLE_unique_objects";


    for(uint32 i = 0; i < debugLayers.size(); ++i){
        bool found = false;
        for(uint32 j = 0; j < layerPropertyCount; ++j){
            if(strcmp(debugLayers[i], props[j].layerName) == 0){
                found = true;
            }
        }
        if(!found){
            NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        }
    }
}
#endif

#endif