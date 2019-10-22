#include "VulkanRenderer.h"
#ifdef _USE_VULKAN


VulkanRenderer::VulkanRenderer()
{
}


VulkanRenderer::~VulkanRenderer()
{
}


void VulkanRenderer::StartUp(Window* window, const GraphicsDesc& desc)
{
    context.Create(*window);
    swapChain.Create(context);
    renderInfo.Create(context, swapChain);
}

void VulkanRenderer::ShutDown()
{
    renderInfo.Destroy(context);
    swapChain.Destroy(context);
    context.Destroy();
}

void VulkanRenderer::RenderBegin()
{

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
