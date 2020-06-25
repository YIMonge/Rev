#ifdef _USE_DIRECTX12

#include "DX12Renderer.h"
#include "Window.h"

void DX12Renderer::StartUp(Window* window, const GraphicsDesc& desc)
{
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

	// Load resource 
	texture.LoadFromFile(&device, "sample_tex.png");
	DX12DescriptorHeap::Chunk resourceChunk = resourceHeap.Allocation(1);
	auto resourceHandle = resourceChunk.GetHandle();
	textureView.Create(&device, texture, &resourceHandle);
	
	resourceChunk = resourceHeap.Allocation(1);
	resourceHandle = resourceChunk.GetHandle();
	constantBufferView.Create(&device, constantBuffer, &resourceHandle);

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
	cbufferOffset.x += 0.005f;
	if (cbufferOffset.x > 1.25f) cbufferOffset.x = -1.25f;
	constantBuffer.Update(cbufferOffset.data, sizeof(revVector4));

	// Render 
	DX12CommandList& globalCommandList = device.GetGlobalCommandList();
	globalCommandList.Open();

	auto& commandList = globalCommandList.GetList();

	rootSiganture.Apply(globalCommandList);
	pipelineState.Apply(globalCommandList);
	// TODO: index detemine by what?
	resourceHeap.Apply(globalCommandList, 0, 1);
	resourceHeap.Apply(globalCommandList, 1, 0);
	samplerHeap.Apply(globalCommandList, 2);
	swapChain.Appply(globalCommandList);

	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, vertexBufferView.GetResourceView());
	commandList->DrawInstanced(3, 1, 0, 0);

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
	vertexShader.LoadFromFile(device, "cbufferTex_vert.hlsl", SHADER_TYPE::VERTX);
	fragmentShader.LoadFromFile(device, "cbufferTex_frag.hlsl", SHADER_TYPE::FRAGMENT); 
	//vertexShader.LoadFromFile(device, "texture_vert.hlsl", SHADER_TYPE::VERTX);
	//fragmentShader.LoadFromFile(device, "texture_frag.hlsl", SHADER_TYPE::FRAGMENT);

	mat.SetShader(SHADER_TYPE::VERTX, &vertexShader);
	mat.SetShader(SHADER_TYPE::FRAGMENT, &fragmentShader);

	struct Vertex
	{
		revVector3 position;
		revVector2 color;
	};

	Vertex triangleVertices[] =
	{
		{ {  0.0f,   0.25f, 0.0f }, { 0.5f, 0.0f } },
		{ {  0.25f, -0.25f, 0.0f }, { 1.0f, 1.0f } },
		{ { -0.25f, -0.25f, 0.0f }, { 0.0f, 1.0f } }
	};

	vertexBuffer.Create(&device, &(triangleVertices[0].position.data[0]), sizeof(Vertex), 3);
	constantBuffer.Create(&device, nullptr, sizeof(revVector4), 1024, revGraphicsBuffer::USAGE::DYNAMIC);

	vertexBufferView.Create(&device, vertexBuffer);
	
	return true;
}

#endif