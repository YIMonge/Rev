#include "revGraphics.h"

#if defined(_USE_DIRECTX12)
	#include "DX12Renderer.h"
	#include "DX12Buffer.h"
#elif defined(_USE_VULKAN)
	#include "VulkanRenderer.h"
	#include "VulkanBuffer.h"
#endif

#include "Window.h"

void revGraphics::StartUp(Window* window, const GraphicsDesc& desc)
{
	this->desc = desc;

#if defined(_USE_OPENGL)
	renderer = new GLRenderer();
#elif defined(_USE_DIRECTX11)
	renderer = new DX11Renderer();
#elif defined(_USE_DIRECTX12)
	renderer = new DX12Renderer();
#elif defined(_USE_VULKAN)
	renderer = new VulkanRenderer();
#else
	renderer = new GLESRenderer();
#endif
	renderer->StartUp(window, desc);
}

void revGraphics::ShutDown()
{
	renderer->ShutDown();
	delete renderer;
	renderer = nullptr;
}

void revGraphics::Draw()
{
	renderer->Render();
}

revGraphicsBuffer* revGraphics::CreateVertexBuffer()
{
	revDevice* device = renderer->GetDevice();
#if defined(_USE_DIRECTX12)
	return new DX12VertexBuffer(device);
#elif defined(_USE_VULKAN)
	return new VulkanVertexBuffer(device);
#endif
}

revGraphicsBuffer* revGraphics::CreateIndexBuffer()
{
	revDevice* device = renderer->GetDevice();
#if defined(_USE_DIRECTX12)
	return new DX12IndexBuffer(device);
#elif defined(_USE_VULKAN)
	return new VulkanIndexBuffer(device);
#endif
}

revGraphicsBuffer* revGraphics::CreateConstantBuffer()
{
	revDevice* device = renderer->GetDevice();
#if defined(_USE_DIRECTX12)
	return new DX12ConstantBuffer(device);
#elif defined(_USE_VULKAN)
	return new VulkanConstantBuffer(device);
#endif
}