#include "revGraphics.h"
#include "revUploadableResource.h"

#if defined(_USE_DIRECTX12)
	#include "DX12Renderer.h"
	#include "DX12Buffer.h"
	#include "DX12MeshRenderer.h"
#elif defined(_USE_VULKAN)
	#include "VulkanRenderer.h"
	#include "VulkanBuffer.h"
	#include "VulkanMeshRenderer.h"
#endif

#include "Window.h"

/// <summary>
/// instantiate and initialize renderer 
/// </summary>
/// <param name="window">window class</param>
/// <param name="desc">graphics desc</param>
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

/// <summary>
/// finalize system
/// </summary>
void revGraphics::ShutDown()
{
	renderer->ShutDown();
	delete renderer;
	renderer = nullptr;
}

/// <summary>
/// upload requested resources to VRAM 
/// </summary>
void revGraphics::Update()
{
	revDevice* device = renderer->GetDevice();
	renderer->OpenGlobalCommandList();
	uint32 count = readyForUploadResources.Count();
	for (uint32 i = 0; i < count; ++i) {
		revUploadableResource* resource = readyForUploadResources.Dequeue();
		resource->Upload(device);
		uploadingResources.Enqueue(resource);
	}
	renderer->CloseGlobalCommandList();
}

void revGraphics::Draw()
{
	renderer->Render();
}

void revGraphics::BeginLoad()
{
	renderer->OpenGlobalCommandList();
}

void revGraphics::EndLoad()
{
	renderer->CloseGlobalCommandList();
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

revMeshRenderer* revGraphics::CreateMeshRenderer()
{
	revDevice* device = renderer->GetDevice();
#if defined(_USE_DIRECTX12)
	return new DX12MeshRenderer();
#elif defined(_USE_VULKAN)
	return new VulkanMeshRenderer();
#endif
}


void revGraphics::Test()
{
#ifdef _USE_DIRECTX12
	((DX12Renderer*)renderer)->TestCode();
#endif
}