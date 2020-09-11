#ifdef _USE_DIRECTX12

#include "DX12Renderer.h"
#include "Window.h"

#include "revResourceManager.h"
#include "FbxLoader.h"
#include "revModelLoader.h"
#include "revTransform.h"
#include "revCamera.h"

#define IRONMAN 1
#define CBUFFER_TEX 0

struct cbuffer
{
	revMatrix44 view;
	revMatrix44 projection;
	revMatrix44 viewProj;
};

cbuffer cbufferData;
revMaterial mat;
revTransform transform;
revCamera camera;

void DX12Renderer::StartUp(Window* window, const GraphicsDesc& desc)
{
	this->window = window;
	if (!device.Create(desc)) return;		
	fence = new DX12Fence(&device);
	if (!fence->Create()) return;

	if (!swapChain.Create(&device, *window)) return;

}

void DX12Renderer::ExecuteCommand(revArray<revGraphicsCommandList>& lists)
{
	uint32 length = static_cast<uint32>(lists.size());
	revArray<ID3D12CommandList*> commandlists(lists.size());
	for (uint32 i = 0; i < length; ++i) {
		commandlists[i] = lists[i].GetList();
	}
	ID3D12CommandQueue* commandQueue = device.GetQueue();
	commandQueue->ExecuteCommandLists(length, commandlists.data());
}

void DX12Renderer::ExecuteCommand(revGraphicsCommandList& list)
{
	ID3D12CommandList* commandLists[] = { list.GetList() };
	ID3D12CommandQueue* commandQueue = device.GetQueue();
	commandQueue->ExecuteCommandLists(1, commandLists);
}

void DX12Renderer::ShutDown()
{
	swapChain.Destroy();
	if (fence) {
		fence->Destroy();
		delete fence;
	}
	device.Destroy();
}

void DX12Renderer::Render()
{
	//meshRenderer.Update();
	//meshRenderer.PrepareDraw(camera);

	// Render 
	DX12CommandList& globalCommandList = device.GetGlobalCommandList();
	globalCommandList.Open();

	auto& commandList = globalCommandList.GetList();

	//rootSiganture.Apply(globalCommandList);
	//pipelineState.Apply(globalCommandList);
	swapChain.Appply(globalCommandList, revColor::BLUE);

//	meshRenderer.Draw(globalCommandList, cbufferHeap, textureHeap, samplerHeap);

	globalCommandList.Close();
	ExecuteCommand(globalCommandList);
	fence->WaitForQueue();
	swapChain.Present();
}

void DX12Renderer::OpenGlobalCommandList()
{
	device.GetGlobalCommandList().Open();
}

void DX12Renderer::CloseGlobalCommandList()
{
	// Render 
	DX12CommandList& globalCommandList = device.GetGlobalCommandList();
	globalCommandList.Close();
}

void DX12Renderer::ExecuteGlobalCommandList()
{
	DX12CommandList& globalCommandList = device.GetGlobalCommandList();
	ExecuteCommand(globalCommandList);
	fence->WaitForQueue();
}

// TEST

#include "revResourceManager.h"
void DX12Renderer::TestCode()
{
	/*
	revResourceManager& resMgr = revResourceManager::Get();
	vertexShader = resMgr.Load<revShader>("ironman_vert");
	fragmentShader = resMgr.Load<revShader>("ironman_frag");
	model = revResourceManager::Get().Load<revModel>("Models/ironman.mdl");

	mat.SetShader(SHADER_TYPE::VERTX, vertexShader);
	mat.SetShader(SHADER_TYPE::FRAGMENT, fragmentShader);

	meshRenderer.SetModel(model);
	meshRenderer.SetMaterialToAllSubMesh(&mat);
	meshRenderer.Initialize(cbufferHeap);

	camera.GetTransform().SetPosition(revVector3(0.0f, 5.0f, -20.0f));
	camera.GetTransform().LookAt(revVector3::ZERO, revVector3::UP);

	revDescriptorBindingDesc rootSignatureDesc;
	rootSignatureDesc.AddMaterial(mat);
	rootSiganture.Create(&device, rootSignatureDesc);
	revRect windowSize(window->GetWidth(), window->GetHeight());
	pipelineState.Create(&device, mat, rootSiganture, windowSize, windowSize);

	texture = resMgr.Load<revTexture>("sample_tex.png");
	DX12DescriptorHeap::Chunk* resourceChunk = GetDescriptorHeap(DESCRIPTOR_HEAP_TYPE::TEXTURE);
	auto resourceHandle = resourceChunk.GetHandle();
	textureView = new DX12TextureView(&device);
	textureView->Create(texture, &resourceHandle);

	DX12DescriptorHeap::Chunk *samplerChunk = GetDescriptorHeap(DESCRIPTOR_HEAP_TYPE::SAMPLER);
	auto samplerHandle = samplerChunk.GetHandle();
	sampler.Create(&device, texture, &samplerHandle);
	*/
}



#endif