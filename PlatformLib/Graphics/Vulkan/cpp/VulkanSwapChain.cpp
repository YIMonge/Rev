#ifdef _USE_VULKAN
#include <string.h>
#include "revArray.h"
#include "VulkanSwapChain.h"
#include "Log.h"


VulkanSwapChain::VulkanSwapChain()
{
    memset(&swapChain, 0, sizeof(swapChain));
}

VulkanSwapChain::~VulkanSwapChain()
{
    memset(&swapChain, 0, sizeof(swapChain));
}

bool VulkanSwapChain::Create(VulkanDevice* _device)
{
    device = _device;
    VkSurfaceCapabilitiesKHR  capabilities;
    const VkPhysicalDevice& gpu = device->GetAdapter();
    const VkSurfaceKHR& surface = device->GetSurface();
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR (gpu, surface, &capabilities);

    uint32 count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &count, nullptr);
    revArray<VkSurfaceFormatKHR> formats(count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &count, formats.data());

    uint32 chosenIndex = 0;
    for(; chosenIndex < count; ++chosenIndex){
        if(formats[chosenIndex].format == VK_FORMAT_R8G8B8A8_UNORM) break;
    }
    if(chosenIndex >= count){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    displaySize = revRect(capabilities.currentExtent.width, capabilities.currentExtent.height);
    format = GRAPHICS_FORMAT ::R8G8B8A8_UNORM;

    uint32 queueFamilyIndex = device->GetQueueFamilyIndex();

    VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.pNext = nullptr;
    swapchainCreateInfo.surface = surface;
    swapchainCreateInfo.minImageCount = capabilities.minImageCount;
    swapchainCreateInfo.imageFormat = ConvertToVKFormat(format);
    swapchainCreateInfo.imageColorSpace = formats[chosenIndex].colorSpace;
    swapchainCreateInfo.imageExtent = capabilities.currentExtent;
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainCreateInfo.queueFamilyIndexCount = 1;
    swapchainCreateInfo.pQueueFamilyIndices = &queueFamilyIndex;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
    swapchainCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
    swapchainCreateInfo.clipped = VK_FALSE;

    const revGraphicsDevice& revDevice = device->GetDevice();
    VkResult result = vkCreateSwapchainKHR(revDevice, &swapchainCreateInfo, nullptr, &swapChain);
    if(result != VK_SUCCESS){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    result = vkGetSwapchainImagesKHR(revDevice, swapChain, &length, nullptr);
    if(result != VK_SUCCESS){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    // create fence and semaphore
    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.pNext = nullptr;
    fenceCreateInfo.flags = 0;
    
    result = vkCreateFence(revDevice, &fenceCreateInfo, nullptr, &fence);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    result = vkResetFences(revDevice, 1, &fence);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
    }

    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.pNext = nullptr;
    semaphoreCreateInfo.flags = 0;
    
    result = vkCreateSemaphore(revDevice, &semaphoreCreateInfo, nullptr, &semaphore);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    return true;
}

bool VulkanSwapChain::CreateFrameBuffer(const VulkanRenderPass& renderPass)
{
    if(!frameBuffers.Create(device, swapChain, format, displaySize, length, renderPass.GetHandle())){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    VkResult result = vkAcquireNextImageKHR(device->GetDevice(),
                                   swapChain,
                                   UINT64_MAX,
                                   semaphore,
                                   VK_NULL_HANDLE,
                                   &frameIndex);

    if(result != VK_SUCCESS){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    return true;
}

void VulkanSwapChain::Destroy()
{
    frameBuffers.Destroy();
    vkDestroySwapchainKHR(device->GetDevice(), swapChain, nullptr);
}

bool VulkanSwapChain::Present()
{
    auto& revDevice = device->GetDevice();
    VkResult result;
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = nullptr;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapChain;
    presentInfo.pImageIndices = &frameIndex;
    presentInfo.waitSemaphoreCount = 0;
    presentInfo.pWaitSemaphores = nullptr;
    presentInfo.pResults = &result;

    vkQueuePresentKHR(device->GetQueue(), &presentInfo);

    // update buffer index
    result = vkAcquireNextImageKHR(revDevice,
                                   swapChain,
                                   UINT64_MAX,
                                   semaphore,
                                   VK_NULL_HANDLE,
                                   &frameIndex);

    // reset fence
    result = vkResetFences(revDevice, 1, &fence);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }

    return result == VK_SUCCESS;
}

bool VulkanSwapChain::WaitForPreviousFrame()
{
    auto& revDevice = device->GetDevice();
    VkResult result;
    // wait for event
    result = vkWaitForFences(revDevice, 1, &fence, VK_TRUE, 100000000);
    if(result != VK_SUCCESS) {
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    return true;
}

#endif
