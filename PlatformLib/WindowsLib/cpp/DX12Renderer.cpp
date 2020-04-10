#include "DX12Renderer.h"
#include "Window.h"


#ifdef _USE_DIRECTX12

void DX12Renderer::StartUp(Window* window, const GraphicsDesc& desc)
{
	if (!device.Create(desc)) return;		
	if (!swapChain.Create(&device, *window)) return;

	IntialzieForApp();

	renderInfo.CreatePipeline(device, vertexShader, fragmentShader);
	rootSiganture.Create(&device);

	// Load resource 
	texture.LoadFromFile(device, "sample_tex.png");
	textureView.Create(device, texture, renderInfo.GetResourceViewHeap());

	// create viewport and scissor 
	rectScissor = { 0, 0, static_cast<LONG>(window->GetWidth()), static_cast<LONG>(window->GetHeight()) };
	viewport = { 0, 0, static_cast<float>(window->GetWidth()), static_cast<float>(window->GetHeight()), 0.0f, 1.0f, };


	// execute initialization task 
	//commandList->Close();
	//ID3D12CommandList* commandLists[] = { commandList };
	//ID3D12CommandQueue* commandQueue = device.GetQueue();
	//commandQueue->ExecuteCommandLists(1, commandLists);
	//swapChain.WaitForPreviousFrame(commandQueue);
}

void DX12Renderer::ShutDown()
{
	swapChain.Destroy();
	device.Destroy();
}

void DX12Renderer::Render()
{
	auto globalCommandList = device.GetGlobalCommandList();
	globalCommandList.Open();

	/*
	auto commandAllocator = device.GetCommandAllocator();;
	auto commandList = device.GetCommandBuffer();
	HRESULT hr = commandAllocator->Reset();
	if (FAILED(hr)) {
		return;
	}
	hr = commandList->Reset(commandAllocator, renderInfo.GetPipelineState());
	if (FAILED(hr)) {
		return;
	}
	*/
	auto commandList = globalCommandList.GetList();

	rootSiganture.Set(globalCommandList);

	auto heap = renderInfo.GetResourceViewHeap();
	commandList->SetDescriptorHeaps(1, &heap);
	commandList->SetGraphicsRootDescriptorTable(0, heap->GetGPUDescriptorHandleForHeapStart());

	commandList->RSSetViewports(1, &viewport);
	commandList->RSSetScissorRects(1, &rectScissor);


	commandList->ResourceBarrier(1, 
		&CD3DX12_RESOURCE_BARRIER::Transition(swapChain.GetCurrentRenderTarget(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET));

	CD3DX12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle = swapChain.GetCPUDescriptorHandle();

	commandList->OMSetRenderTargets(1, &renderTargetViewHandle, FALSE, nullptr);

	const float clearColor[] = { 0.0f, 0.2f, 1.0f, 1.0f };
	commandList->ClearRenderTargetView(renderTargetViewHandle, clearColor, 0, nullptr);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, vertexBuffer.GetResourceView());
	commandList->DrawInstanced(3, 1, 0, 0);


	commandList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(swapChain.GetCurrentRenderTarget(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT));


	hr = commandList->Close();	
	if (FAILED(hr)) {
		return;
	}

	ID3D12CommandList* commandLists[] = { commandList };
	ID3D12CommandQueue* commandQueue = device.GetQueue();
	commandQueue->ExecuteCommandLists(1, commandLists);
	swapChain.Present();

	// wait for current frame 
	swapChain.WaitForPreviousFrame(commandQueue);	
}

#include "File.h"
revMaterial mat;

bool DX12Renderer::IntialzieForApp()
{
	// TODO: load shader 
	vertexShader.LoadFromFile(device, "texture.hlsl", SHADER_TYPE::VERTX);
	fragmentShader.LoadFromFile(device, "texture.hlsl", SHADER_TYPE::FRAGMENT);

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