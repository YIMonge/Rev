#ifdef _USE_DIRECTX12

#include "DX12Renderer.h"
#include "Window.h"

#include "FbxLoader.h"

struct cbuffer
{
	revMatrix44 world;
	revMatrix44 view;
	revMatrix44 projection;
	revMatrix44 wvp;
};

cbuffer cbufferData;

void DX12Renderer::StartUp(Window* window, const GraphicsDesc& desc)
{
	main_window = window;
	if (!device.Create(desc)) return;		
	if (!resourceHeap.Create(&device, DESCRIPTOR_HEAP_TYPE::RESOURCE, 1024)) return;
	if (!samplerHeap.Create(&device, DESCRIPTOR_HEAP_TYPE::SAMPLER, 128)) return;
	if (!swapChain.Create(&device, *window)) return;

	IntialzieForApp();

	revDescriptorBindingDesc rootSignatureDesc;
	rootSignatureDesc.AddMaterial(mat);
	rootSiganture.Create(&device, rootSignatureDesc);
	revRect windowSize(window->GetWidth(), window->GetHeight());
	pipelineState.Create(&device, mat, rootSiganture, windowSize, windowSize);


	DX12DescriptorHeap::Chunk resourceChunk = resourceHeap.Allocation(1);
	auto resourceHandle = resourceChunk.GetHandle();
	constantBufferView.Create(&device, *constantBuffer, &resourceHandle);

	// Load resource 
	texture.LoadFromFile(&device, "sample_tex.png");
	resourceChunk = resourceHeap.Allocation(1);
	resourceHandle = resourceChunk.GetHandle();
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
	swapChain.Destroy();
	device.Destroy();
}

void DX12Renderer::Render()
{
	// Update
	//cbufferOffset.x += 0.005f;
	//if (cbufferOffset.x > 1.25f) cbufferOffset.x = -1.25f;
	//constantBuffer->Update(cbufferOffset.data, sizeof(revVector4));

	// Render 
	DX12CommandList& globalCommandList = device.GetGlobalCommandList();
	globalCommandList.Open();

	auto& commandList = globalCommandList.GetList();

	rootSiganture.Apply(globalCommandList);
	pipelineState.Apply(globalCommandList);
	// TODO: index detemine by what?
	resourceHeap.Apply(globalCommandList, 0, 0);
	resourceHeap.Apply(globalCommandList, 1, 1);
	samplerHeap.Apply(globalCommandList, 2);
	swapChain.Appply(globalCommandList);

	meshRenderer.Draw(globalCommandList);

	globalCommandList.Close();
	ExecuteCommand(globalCommandList);
	swapChain.Present();
	swapChain.WaitForPreviousFrame();
	globalCommandList.ReleaseResoucers();
}

#include "File.h"
revMaterial mat;

bool DX12Renderer::IntialzieForApp()
{
	//vertexShader.LoadFromFile(device, "cbuffer_vert.hlsl", SHADER_TYPE::VERTX);
	//fragmentShader.LoadFromFile(device, "cbuffer_frag.hlsl", SHADER_TYPE::FRAGMENT);
	//vertexShader.LoadFromFile(device, "cbufferTex_vert.hlsl", SHADER_TYPE::VERTX);
	//fragmentShader.LoadFromFile(device, "cbufferTex_frag.hlsl", SHADER_TYPE::FRAGMENT); 
	//vertexShader.LoadFromFile(device, "texture_vert.hlsl", SHADER_TYPE::VERTX);
	//fragmentShader.LoadFromFile(device, "texture_frag.hlsl", SHADER_TYPE::FRAGMENT);
	vertexShader.LoadFromFile(device, "model_vert.hlsl", SHADER_TYPE::VERTX);
	fragmentShader.LoadFromFile(device, "model_frag.hlsl", SHADER_TYPE::FRAGMENT);

	mat.SetShader(SHADER_TYPE::VERTX, &vertexShader);
	mat.SetShader(SHADER_TYPE::FRAGMENT, &fragmentShader);

	FBXLoader loader;
	//loader.LoadFromFile("../../Resources/Models/plane.fbx", &model);
	//loader.LoadFromFile("../../Resources/Models/cube_maya.fbx", &model);
	loader.LoadFromFile("../../Resources/Models/cube_blender.fbx", &model);
	//	loader.LoadFromFile("../../Resources/Models/ironman.fbx", &mesh2);
	meshRenderer.SetMeshes(model.GetMeshes());
	meshRenderer.SetMaterial(0, &mat);
	meshRenderer.Initialize();

	cbufferData.view.CreateLookAtMatrixLH(revVector3(0.0f, 0.0f, -10.0f), revVector3(0.0f, 0.0f, 0.0f), revVector3(0.0f, 1.0f, 0.0f));
	cbufferData.projection.CreatePerspectiveMatrixLH(MathUtil::ToRadian(45.0f), main_window->GetAspectRatio() , 0.001f, 100.0f);
	cbufferData.wvp.Identity();
	cbufferData.wvp = cbufferData.view * cbufferData.projection;
	cbufferData.wvp.Transpose();

	constantBuffer = new DX12ConstantBuffer(&device);
	constantBuffer->Create(&cbufferData, sizeof(cbufferData), 1, revGraphicsBuffer::USAGE::DYNAMIC);

	return true;
}



#endif