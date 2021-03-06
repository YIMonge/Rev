#ifndef __VULKANRENDERPASS_H__
#define __VULKANRENDERPASS_H__
#ifdef _USE_VULKAN

#include "revDevice.h"
#include "revMaterial.h"
#include "revRect.h"
#include "VulkanCommandList.h"

class VulkanSwapChain;

class VulkanRenderPass
{
public:
    VulkanRenderPass();
    ~VulkanRenderPass();

    bool Create(revDevice* device, VulkanSwapChain* swapChain, const revMaterial& material);
    void Destroy();

    void Begin(VulkanCommandList& commandList, const VkFramebuffer& framebuffer, const revColor& clearColor, const revRect& viewport);
    void End(VulkanCommandList& commandList);

    const VkRenderPass& GetHandle() const { return renderPass; }

private:
    revDevice* device;
    VkRenderPass renderPass;
    VkRenderPassBeginInfo renderPassBeginInfo;
};

#endif
#endif

