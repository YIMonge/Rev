#include <string.h>
#include "revArray.h"
#include "VulkanSwapChain.h"

#ifdef _USE_VULKAN

VulkanSwapChain::VulkanSwapChain()
{
    memset(&swapChain, 0, sizeof(swapChain));
}

VulkanSwapChain::~VulkanSwapChain()
{
}

bool VulkanSwapChain::Create(const VulkanDeviceContext& deviceContext)
{
    VkSurfaceCapabilitiesKHR  capabilities;
    const VkPhysicalDevice& gpu = deviceContext.GetGpuDevice();
    const VkSurfaceKHR& surface = deviceContext.GetSurface();
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR (gpu, surface, &capabilities);

    uint32 count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &count, nullptr);
    revArray<VkSurfaceFormatKHR> formats(count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &count, &formats[0]);

    uint32 chosenIndex = 0;
    for(; chosenIndex < count; ++chosenIndex){
        if(formats[chosenIndex].format == VK_FORMAT_R8G8B8A8_UNORM) break;
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
            .presentMode = VK_PRESENT_MODE_FIFO_KHR,
            .oldSwapchain = VK_NULL_HANDLE,
            .clipped = VK_FALSE,
    };

    const VkDevice& device = deviceContext.GetDevice();
    vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapChain);
    vkGetSwapchainImagesKHR(device, swapChain, &length, nullptr);

    return true;
}

void VulkanSwapChain::Destroy(const VulkanDeviceContext& deviceContext)
{
    const VkDevice& device = deviceContext.GetDevice();
    for(int i = 0; i < length; ++i) {
        vkDestroyFramebuffer(device, frameBuffers[i], nullptr);
        vkDestroyImageView(device, views[i], nullptr);
        vkDestroyImage(device, images[i], nullptr);
    }
    vkDestroySwapchainKHR(device, swapChain, nullptr);
}

#endif
