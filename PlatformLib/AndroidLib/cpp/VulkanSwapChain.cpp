#include <string.h>
#include "revArray.h"
#include "VulkanSwapChain.h"
#include "Log.h"

#ifdef _USE_VULKAN

VulkanSwapChain::VulkanSwapChain()
{
    memset(&swapChain, 0, sizeof(swapChain));
}

VulkanSwapChain::~VulkanSwapChain()
{
    memset(&swapChain, 0, sizeof(swapChain));
}

bool VulkanSwapChain::Create(const VulkanDeviceContext& deviceContext)
{
    VkSurfaceCapabilitiesKHR  capabilities;
    const VkPhysicalDevice& gpu = deviceContext.GetGpuDevice();
    const VkSurfaceKHR& surface = deviceContext.GetSurface();
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR (gpu, surface, &capabilities);

    uint32 count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &count, nullptr);
    VkSurfaceFormatKHR formats[count];
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &count, formats);

    uint32 chosenIndex = 0;
    for(; chosenIndex < count; ++chosenIndex){
        if(formats[chosenIndex].format == VK_FORMAT_R8G8B8A8_UNORM) break;
    }
    if(chosenIndex >= count){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    displaySize = capabilities.currentExtent;
    format = formats[chosenIndex].format;

    uint32 queueFamilyIndex = deviceContext.GetQueueFamilyIndex();

    VkSwapchainCreateInfoKHR swapchainCreateInfo{
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext = nullptr,
            .surface = surface,
            .minImageCount = capabilities.minImageCount,
            .imageFormat = format,
            .imageColorSpace = formats[chosenIndex].colorSpace,
            .imageExtent = capabilities.currentExtent,
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
            .imageArrayLayers = 1,
            .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 1,
            .pQueueFamilyIndices = &queueFamilyIndex,
            .compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
            .presentMode = VK_PRESENT_MODE_FIFO_KHR,
            .oldSwapchain = VK_NULL_HANDLE,
            .clipped = VK_FALSE,
    };

    const VkDevice& device = deviceContext.GetDevice();
    VkResult result = vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapChain);
    if(result != VK_SUCCESS){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    result = vkGetSwapchainImagesKHR(device, swapChain, &length, nullptr);
    if(result != VK_SUCCESS){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    return true;
}

void VulkanSwapChain::Destroy(const VulkanDeviceContext& deviceContext)
{
    vkDestroySwapchainKHR(deviceContext.GetDevice(), swapChain, nullptr);
}

#endif
