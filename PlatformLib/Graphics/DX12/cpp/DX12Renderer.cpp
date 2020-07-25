#ifdef _USE_DIRECTX12

#include "DX12Renderer.h"
#include "Window.h"

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
	main_window = window;
	if (!device.Create(desc)) return;		
	if (!cbufferHeap.Create(&device, DESCRIPTOR_HEAP_TYPE::BUFFER, 1024)) return;
	if (!textureHeap.Create(&device, DESCRIPTOR_HEAP_TYPE::TEXTURE, 1024)) return;
	if (!samplerHeap.Create(&device, DESCRIPTOR_HEAP_TYPE::SAMPLER, 128)) return;
	if (!swapChain.Create(&device, *window)) return;

	IntialzieForApp();

	revDescriptorBindingDesc rootSignatureDesc;
	rootSignatureDesc.AddMaterial(mat);
	rootSiganture.Create(&device, rootSignatureDesc);
	revRect windowSize(window->GetWidth(), window->GetHeight());
	pipelineState.Create(&device, mat, rootSiganture, windowSize, windowSize);

	// Load resource 
	texture.LoadFromFile(&device, "sample_tex.png");
	DX12DescriptorHeap::Chunk resourceChunk = textureHeap.Allocation(1);
	auto resourceHandle = resourceChunk.GetHandle();
	textureView.Create(&device, texture, &resourceHandle);
	

	DX12DescriptorHeap::Chunk samplerChunk = samplerHeap.Allocation(1);
	auto samplerHandle = samplerChunk.GetHandle();
	sampler.Create(&device, texture, &samplerHandle);

	DX12CommandList& commandList = device.GetGlobalCommandList();
	commandList.Close();
	ExecuteCommand(commandList);
	swapChain.WaitForPreviousFrame();
	commandList.ReleaseResoucers();
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
	meshRenderer.Destroy();
	swapChain.Destroy();
	device.Destroy();
}

void DX12Renderer::Render()
{
	meshRenderer.Update();
	meshRenderer.PrepareDraw(camera);

	// Render 
	DX12CommandList& globalCommandList = device.GetGlobalCommandList();
	globalCommandList.Open();

	auto& commandList = globalCommandList.GetList();

	rootSiganture.Apply(globalCommandList);
	pipelineState.Apply(globalCommandList);
	swapChain.Appply(globalCommandList, revColor::BLUE);

	meshRenderer.Draw(globalCommandList, cbufferHeap, textureHeap, samplerHeap);

	globalCommandList.Close();
	ExecuteCommand(globalCommandList);
	swapChain.Present();
	swapChain.WaitForPreviousFrame();
	globalCommandList.ReleaseResoucers();
}

bool DX12Renderer::IntialzieForApp()
{

	vertexShader.LoadFromFile(device, "ironman_vert.hlsl", SHADER_TYPE::VERTX);
	fragmentShader.LoadFromFile(device, "ironman_frag.hlsl", SHADER_TYPE::FRAGMENT);
	FBXLoader loader;
	loader.LoadFromFile("Models/ironman.fbx", &model);

	//revModelLoader loader;
	//loader.LoadFromFile("Models/ironman.mdl", &model);
	//loader.LoadFromFile("Models/cube_blender.mdl", &model);



	mat.SetShader(SHADER_TYPE::VERTX, &vertexShader);
	mat.SetShader(SHADER_TYPE::FRAGMENT, &fragmentShader);

	meshRenderer.SetModel(&model);
	//meshRenderer.SetMeshes(model.GetMeshes());
	meshRenderer.SetMaterialToAllSubMesh(&mat);
	meshRenderer.Initialize(&cbufferHeap);

	camera.GetTransform().SetPosition(revVector3(0.0f, 5.0f, -20.0f));
	camera.GetTransform().LookAt(revVector3::ZERO, revVector3::UP);

	return true;
}



#endif