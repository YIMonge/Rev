#ifdef _USE_DIRECTX12 

#include "DX12RenderInfo.h"

DX12RenderInfo::DX12RenderInfo()
{
}

DX12RenderInfo::~DX12RenderInfo()
{
}



bool DX12RenderInfo::CreatePipeline(const DX12Device& deviceContext, const DX12Shader& vertexShader, const DX12Shader& fragmentShader)
{
	/*
	// TODO: vertex shader, pixel shader, vertex format 
	// create pipeline state 
	D3D12_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.FillMode = static_cast<D3D12_FILL_MODE>(POLYGON_MODE::FILL);
	rasterizerDesc.CullMode = static_cast<D3D12_CULL_MODE>(CULL_MODE_FLAG::BACK);
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.ForcedSampleCount = 0;
	rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc;
	renderTargetBlendDesc.BlendEnable = false;
	renderTargetBlendDesc.LogicOpEnable = false;
	renderTargetBlendDesc.SrcBlend = D3D12_BLEND_ONE;
	renderTargetBlendDesc.DestBlend = D3D12_BLEND_ZERO;
	renderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	renderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	renderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	renderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	renderTargetBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
	renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;


	D3D12_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	for (uint32 i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) blendDesc.RenderTarget[i] = renderTargetBlendDesc;

	//TODO: choose by material 
	D3D12_INPUT_ELEMENT_DESC inputElements[] = {
			{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			//{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			//{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc;
	memset(&pipelineStateDesc, 0, sizeof(pipelineStateDesc));
	pipelineStateDesc.InputLayout = { inputElements, _countof(inputElements) };
	pipelineStateDesc.pRootSignature = rootSignature;
	pipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.GetHandle());
	pipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(fragmentShader.GetHandle());
	pipelineStateDesc.RasterizerState = rasterizerDesc;
	pipelineStateDesc.BlendState = blendDesc;
	pipelineStateDesc.DepthStencilState.DepthEnable = false;
	pipelineStateDesc.DepthStencilState.StencilEnable = false;
	pipelineStateDesc.SampleMask = UINT_MAX;
	pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipelineStateDesc.NumRenderTargets = 1;
	pipelineStateDesc.RTVFormats[0] = ConvertToDXFormat(GRAPHICS_FORMAT::R8G8B8A8_UNORM);
	pipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	pipelineStateDesc.SampleDesc.Count = 1;

	auto device = deviceContext.GetDevice();
	HRESULT hr = device->CreateGraphicsPipelineState(&pipelineStateDesc, IID_PPV_ARGS(&pipelineState));
	if (FAILED(hr)) {
		return false;
	}


	D3D12_DESCRIPTOR_HEAP_DESC  descriptorHeapDesc = {};
	descriptorHeapDesc.NumDescriptors = 1;
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	hr = device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&resourceViewHeap));
	if (FAILED(hr)) {
		return false;
	}

	return true;
	*/
	return true;
}




bool DX12RenderInfo::CreatePipeline(const DX12Device& deviceContext, const revMaterial& material)
{




	return true;
}

#endif
