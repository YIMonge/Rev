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
}

void VkRenderer::ShutDown()
{
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
