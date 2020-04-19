#include "DX12Renderer.h"
#include "Window.h"


#ifdef _USE_DIRECTX12

void DX12Renderer::StartUp(Window* window, const GraphicsDesc& desc)
{
	if (!device.Create(desc)) return;		
	if (!swapChain.Create(&device, *window)) return;

	IntialzieForApp();

	//renderInfo.CreatePipeline(device, vertexShader, fragmentShader);
	rootSiganture.Create(&device);
	pipelineState.Create(&device, mat, rootSiganture);

	// Load resource 
	texture.LoadFromFile(&device, "sample_tex.png");
	textureView.Create(device, texture, pipelineState.GetResourceViewHeap());

	// create viewport and scissor 
	rectScissor = { 0, 0, static_cast<LONG>(window->GetWidth()), static_cast<LONG>(window->GetHeight()) };
	viewport = { 0, 0, static_cast<float>(window->GetWidth()), static_cast<float>(window->GetHeight()), 0.0f, 1.0f, };

	DX12CommandList& commandList = device.GetGlobalCommandList();
	commandList.Close();
	ExecuteCommand(commandList);
	swapChain.WaitForPreviousFrame(device.GetQueue());
	commandList.ReleaseResoucers();
}

void DX12Renderer::ExecuteCommand(revArray<revGraphicsCommandList>& lists)
{
	uint32 length = lists.size();
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
	DX12CommandList& globalCommandList = device.GetGlobalCommandList();
	globalCommandList.Open();

	auto& commandList = globalCommandList.GetList();

	rootSiganture.Apply(globalCommandList);
	pipelineState.Apply(globalCommandList);
	commandList->RSSetViewports(1, &viewport);
	commandList->RSSetScissorRects(1, &rectScissor);
	swapChain.Appply(globalCommandList);

	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, vertexBuffer.GetResourceView());
	commandList->DrawInstanced(3, 1, 0, 0);

	globalCommandList.Close();
	ExecuteCommand(globalCommandList);
	swapChain.Present();
	swapChain.WaitForPreviousFrame(device.GetQueue());
	globalCommandList.ReleaseResoucers();
}

#include "File.h"
revMaterial mat;

bool DX12Renderer::IntialzieForApp()
{
	// TODO: load shader 
	vertexShader.LoadFromFile(device, "texture.hlsl", SHADER_TYPE::VERTX);
	fragmentShader.LoadFromFile(device, "texture.hlsl", SHADER_TYPE::FRAGMENT);

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

	vertexBuffer.Create(device, &(triangleVertices[0].position.data[0]), sizeof(triangleVertices));
	
	return true;
}

#endif