#include "../include/DX12Renderer.h"
#include "../include/Window.h"


#if defined(_USE_DIRECTX12)

void DX12Renderer::StartUp(Window* window, const GraphicsDesc& desc)
{
	bool result = deviceContext.Create(desc);
	if(!result){
		
		return;
	}
	swapChain.Create(deviceContext, *window);

	auto device = deviceContext.GetDevice();	
	HRESULT hr;
	// create allocator and command list 
	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	if (FAILED(hr)) {
		return;
	}
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, IID_PPV_ARGS(&commandList));
	if (FAILED(hr)) {
		return;
	}
	commandList->Close();

	// create fence 
	hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	if (FAILED(hr)) {
		return;
	}
	fenceValue = 1;
	fenceEvent = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
	if (fenceEvent == nullptr) {
		return;
	}


	// create viewport and scissor 
	rectScissor = { 0, 0, static_cast<LONG>(window->GetWidth()), static_cast<LONG>(window->GetHeight()) };
	viewport = { 0, 0, static_cast<float>(window->GetWidth()), static_cast<float>(window->GetHeight()), 0.0f, 1.0f, };
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

void DX12Renderer::RenderBegin()
{
	commandAllocator->Reset();
	//commandList->Reset(commandAllocator, pipelineState);
	//commandList->SetDescriptorHeaps(1, heap)
	//commandList->RSSetViewports(1, &viewport);
	
}

bool DX12Renderer::IntialzieForApp()
{
	// create root signature 
	D3D12_DESCRIPTOR_RANGE descriptorRange;
	descriptorRange.RangeType			= D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	descriptorRange.NumDescriptors		= 1;
	descriptorRange.BaseShaderRegister	= 0;
	descriptorRange.RegisterSpace		= 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootParam;
	rootParam.ParameterType		= D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParam.ShaderVisibility	= D3D12_SHADER_VISIBILITY_VERTEX;
	rootParam.DescriptorTable.NumDescriptorRanges	= 1;
	rootParam.DescriptorTable.pDescriptorRanges		= &descriptorRange;

	D3D12_ROOT_SIGNATURE_DESC signatureDesc;
	signatureDesc.NumParameters		= 1;
	signatureDesc.pParameters		= &rootParam;
	signatureDesc.NumStaticSamplers = 0;
	signatureDesc.pStaticSamplers	= nullptr;
	signatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
		| D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS
		| D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS
		| D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS
		| D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

	ID3DBlob* signature;
	ID3DBlob* error;

	HRESULT hr = D3D12SerializeRootSignature(&signatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&signature,
		&error);
	if (FAILED(hr)) {
		return false;
	}

	auto device = deviceContext.GetDevice();
	hr = device->CreateRootSignature(0,
		signature->GetBufferPointer(),
		signature->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	if (FAILED(hr)) {
		return false;
	}

	// TODO: vertex shader, pixel shader, vertex format 
	// create pipeline state 
	D3D12_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.ForcedSampleCount = 0;
	rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {
		false, false, 
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP,
		D3D12_COLOR_WRITE_ENABLE_ALL
	};

	D3D12_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	for (uint32 i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) blendDesc.RenderTarget[i] = renderTargetBlendDesc;


	D3D12_INPUT_ELEMENT_DESC inputElements[] = {
			{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "VTX_COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc;
	memset(&pipelineStateDesc, 0, sizeof(pipelineStateDesc));
	pipelineStateDesc.InputLayout = { inputElements, _countof(inputElements) };
	pipelineStateDesc.pRootSignature = rootSignature;
	// TODO
	// pipelineStateDesc.VS
	// pipelineStateDesc.PS
	pipelineStateDesc.RasterizerState = rasterizerDesc;
	pipelineStateDesc.BlendState = blendDesc;
	pipelineStateDesc.DepthStencilState.DepthEnable = false;
	pipelineStateDesc.DepthStencilState.StencilEnable = false;
	pipelineStateDesc.SampleMask = UINT_MAX;
	pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipelineStateDesc.NumRenderTargets = 1;
	pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	pipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	pipelineStateDesc.SampleDesc.Count = 1;
	
	hr = device->CreateGraphicsPipelineState(&pipelineStateDesc, IID_PPV_ARGS(&pipelineState));
	if (FAILED(hr)) {
		return false;
	}

	return true;
}

#endif