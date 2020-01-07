#include "VulkanRenderer.h"
#include "Log.h" // AndroidLib doesn't depend on rev
#ifdef _USE_VULKAN


VulkanRenderer::VulkanRenderer()
{
}


VulkanRenderer::~VulkanRenderer()
{
}


void VulkanRenderer::StartUp(Window* window, const GraphicsDesc& desc)
{
    if(!context.Create(*window)) return;
    if(!swapChain.Create(context)) return;
    if(!renderInfo.Create(context, swapChain)) return;
    if(!frameBuffer.Create(context, swapChain, renderInfo)) return;
}

void VulkanRenderer::ShutDown()
{
    frameBuffer.Destroy(context);
    renderInfo.Destroy(context);
    swapChain.Destroy(context);
    context.Destroy();
}

void VulkanRenderer::RenderBegin()
{
    const VkDevice& device = context.GetDevice();
    const VkSwapchainKHR swapchainKhr = swapChain.GetSwapChain();
    // TEST code
    //vkAcquireNextImageKHR(device, swapchainKhr, UINT64_MAX, )

}

void VulkanRenderer::RenderEnd()
{

}

void VulkanRenderer::Clear(bool clear_color, bool clear_depth, const revColor& fill_color)
{

}

void VulkanRenderer::SwapBuffers()
{

}

void VulkanRenderer::SetBlendFunc(BLEND_FUNC func)
{

}

void VulkanRenderer::SetAlphaTest(ALPHA_TEST func, float value)
{

}

#endif
