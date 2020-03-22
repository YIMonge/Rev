#include "../include/DX12Renderer.h"
#include "../include/Window.h"


#ifdef _USE_DIRECTX12

void DX12Renderer::StartUp(Window* window, const GraphicsDesc& desc)
{
	if (!deviceContext.Create(desc)) return;		
	if (!swapChain.Create(deviceContext, *window)) return;

	auto device = deviceContext.GetDevice();	
	HRESULT hr;

	// create viewport and scissor 
	rectScissor = { 0, 0, static_cast<LONG>(window->GetWidth()), static_cast<LONG>(window->GetHeight()) };
	viewport = { 0, 0, static_cast<float>(window->GetWidth()), static_cast<float>(window->GetHeight()), 0.0f, 1.0f, };


	IntialzieForApp();


	renderInfo.Create(deviceContext, vertexShader, fragmentShader);

	// create allocator and command list 
	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	if (FAILED(hr)) {
		return;
	}
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, renderInfo.GetPipelineState(), IID_PPV_ARGS(&commandList));
	if (FAILED(hr)) {
		return;
	}
	commandList->Close();
}

void DX12Renderer::ShutDown()
{
	if (commandList != nullptr) {
		commandList->Release();
		commandList = nullptr;
	}

	if (commandAllocator != nullptr) {
		commandAllocator->Release();
		commandAllocator = nullptr;
	}

	swapChain.Destroy();
	deviceContext.Destroy();
}

void DX12Renderer::Render()
{
	HRESULT hr = commandAllocator->Reset();
	if (FAILED(hr)) {
		return;
	}
	hr = commandList->Reset(commandAllocator, renderInfo.GetPipelineState());
	if (FAILED(hr)) {
		return;
	}
	
	commandList->SetGraphicsRootSignature(renderInfo.GetRootSignature());
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
	ID3D12CommandQueue* commandQueue = deviceContext.GetCommandQueue();
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
	vertexShader.LoadFromFile(deviceContext, "Resources\\shaders.hlsl", SHADER_TYPE::VERTX);
	fragmentShader.LoadFromFile(deviceContext, "Resources\\shaders.hlsl", SHADER_TYPE::FRAGMENT);

	struct Vertex
	{
		revVector4 position;
		revVector4 color;
	};

	Vertex triangleVertices[] =
	{
		{ {  0.0f,   0.25f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ {  0.25f, -0.25f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { -0.25f, -0.25f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
	};

	vertexBuffer.Create(deviceContext, &(triangleVertices[0].position.data[0]), sizeof(triangleVertices), REV_GRAPHICS_BUFFER_FOMAT_FLAG::VERTEX);

	//File file("Resources\\mat.meta", FileMode::ReadText);
	// Test material make 
/*
	revMaterial::BlendState blend;
	revMaterial::RasterizationState rasterizatin;

	blend.SetEnableBlend(false);
	blend.SetBlendFactorSrcColor(BLEND_FACTOR::ONE);
	blend.SetBlendFactorDstColor(BLEND_FACTOR::ZERO);
	blend.SetBlendOpColor(BLEND_OP::ADD);
	blend.SetBlendFactorSrcAlpha(BLEND_FACTOR::ONE);
	blend.SetBlendFactorDstAlpha(BLEND_FACTOR::ZERO);
	blend.SetBlendOpAlpha(BLEND_OP::ADD);

	rasterizatin.SetCullMode(CULL_MODE_FLAG::BACK);

	revSerializer::Serialize("Resources\\material.mat", mat);
*/
//	revSerializer::Deserialize("Resources\\material.mat", mat);


	return true;
}

#endif