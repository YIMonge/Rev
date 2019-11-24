#include "../include/DX12Renderer.h"
#include "../include/Window.h"


#if defined(_USE_DIRECTX12)

void DX12Renderer::StartUp(Window* window, const GraphicsDesc& desc)
{
	bool result = deviceContext.Create(desc);
	if(!result){
		
	}
	swapChain.Create(deviceContext, *window);

	uint32 bufferNum = desc.GetBufferNum();
	commandAllocators.Resize(bufferNum);
	commandLists.Resize(bufferNum);

	auto device = deviceContext.GetDevice();
	
	// create allocator and command list 
	for (uint32 i = 0; i < bufferNum; ++i) {
		device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocators[i]));
		device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocators[i], nullptr, IID_PPV_ARGS(&commandLists[i]));
		commandLists[i]->Close();
	}

	// create viewport and scissor 
	rectScissor = { 0, 0, window->GetWidth(), window->GetHeight() };
	viewport = { 0, 0, static_cast<float>(window->GetWidth()), static_cast<float>(window->GetHeight()), 0.0f, 1.0f, };


}

void DX12Renderer::ShutDown()
{
	uint32 bufferNum = deviceContext.GetDesc().GetBufferNum();
	for (uint32 i = 0; i < bufferNum; ++i) {
		if (commandLists[i] != nullptr) {
			commandLists[i]->Release();
			commandLists[i] = nullptr;
		}

		if (commandAllocators[i] != nullptr) {
			commandAllocators[i]->Release();
			commandAllocators[i] = nullptr;
		}
	}


	swapChain.Destroy();
	deviceContext.Destroy();
}

#endif