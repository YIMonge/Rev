#ifndef __VULKANDEVICE_H__
#define __VULKANDEVICE_H__
#ifdef _USE_VULKAN

#include "revDevice.h"
#ifdef _DEBUG
#include "revArray.h"
#endif
#include "VulkanCommandList.h"

class Window;

class VulkanDevice : public revDevice
{
public:
    VulkanDevice(){}
    virtual ~VulkanDevice(){}

    bool Create(Window* window);
    bool CreateCommandList(uint32 num);
    void Destroy();

    VkInstance GetInstance() const { return instance; }
    VkSurfaceKHR GetSurface() const { return surface; }
    uint32 GetQueueFamilyIndex() const { return queueFamilyIndex; }
    const uint32* GetQueueFamilyIndexPtr() const { return &queueFamilyIndex; }
    const VkPhysicalDeviceMemoryProperties& GetPhysicalDeviceMemoryProperties() const { return physicalDeviceMemoryProperties; }

    VulkanCommandList& GetGlobalCommandList() { return globalCommandList; }
    revArray<VulkanCommandList>& GetCommandLists() { return commandLists; }

private:
    VkInstance instance;
    VkSurfaceKHR surface;
    uint32 queueFamilyIndex;
    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
    VulkanCommandList globalCommandList;
    revArray<VulkanCommandList> commandLists;
#ifdef _DEBUG
    void initializeDebugLayer();
    revArray<const char*> debugLayers;
#endif
};

#endif
#endif
