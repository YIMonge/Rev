#include "VkRenderer.h"
#ifdef _USE_VULKAN


VkRenderer::VkRenderer()
{
}


VkRenderer::~VkRenderer()
{
}


void VkRenderer::StartUp(Window* window, const GraphicsDesc& desc)
{
    context.Create(*window);
    swapChain.Create(context);
}

void VkRenderer::ShutDown()
{
    swapChain.Destroy();
    context.Destroy();
}

void VkRenderer::RenderBegin()
{

}

void VkRenderer::RenderEnd()
{

}

void VkRenderer::Clear(bool clear_color, bool clear_depth, const revColor& fill_color)
{

}

void VkRenderer::SwapBuffers()
{

}

void VkRenderer::SetBlendFunc(BLEND_FUNC func)
{

}

void VkRenderer::SetAlphaTest(ALPHA_TEST func, float value)
{

}

#endif
