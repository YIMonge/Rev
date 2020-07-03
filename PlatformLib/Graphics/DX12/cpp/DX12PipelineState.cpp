#ifdef _USE_DIRECTX12
#include "DX12PipelineState.h"

bool DX12PipelineState::Create(revDevice* device, const revMaterial& material, const DX12RootSignature& rootSignature, const revRect& viewportRect, const revRect& scissorRect)
{
	const auto& blendState = material.GetBlendState();
	const auto& rasterizerState = material.GetRasterization();
	const auto& depthStencil = material.GetDepthStencil();

	D3D12_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.FillMode = ConvertToDXFillMode(rasterizerState.GetPolygonMode());
	rasterizerDesc.CullMode = ConvertToDXCullMode(rasterizerState.GetCullMode());
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterizerDesc.DepthClipEnable = depthStencil.IsEnableDepthTest();
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.ForcedSampleCount = 0;
	rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc;
	renderTargetBlendDesc.BlendEnable = blendState.isEnableBlend();
	renderTargetBlendDesc.LogicOpEnable = blendState.isEnabledLogicOp();
	renderTargetBlendDesc.SrcBlend = ConvertToDXBlendFactor(blendState.GetBlendFactorSrcColor());			//D3D12_BLEND_ONE;
	renderTargetBlendDesc.DestBlend = ConvertToDXBlendFactor(blendState.GetBlendFactorDstColor());			//D3D12_BLEND_ZERO;
	renderTargetBlendDesc.BlendOp = ConvertToDXBlendOp(blendState.GetBlendOpColor());						//D3D12_BLEND_OP_ADD;
	renderTargetBlendDesc.SrcBlendAlpha = ConvertToDXBlendFactor(blendState.GetBlendFactorSrcAlpha());		//D3D12_BLEND_ONE;
	renderTargetBlendDesc.DestBlendAlpha = ConvertToDXBlendFactor(blendState.GetBlendFactorDstAlpha());		//D3D12_BLEND_ZERO;
	renderTargetBlendDesc.BlendOpAlpha = ConvertToDXBlendOp(blendState.GetBlendOpAlpha());					//D3D12_BLEND_OP_ADD;
	renderTargetBlendDesc.LogicOp = ConvertToDXLogicOp(blendState.GetLogicOp());							 //D3D12_LOGIC_OP_NOOP;
	renderTargetBlendDesc.RenderTargetWriteMask = ConvertToDXColorComponent(blendState.GetWriteColorMask()); //D3D12_COLOR_WRITE_ENABLE_ALL;

	D3D12_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = blendState.isEbaleAlphaToCoverage();
	blendDesc.IndependentBlendEnable = false; // TODO: set from material
	for (uint32 i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
		blendDesc.RenderTarget[i] = renderTargetBlendDesc;
	}

	revArray<D3D12_INPUT_ELEMENT_DESC> inputElements;
	auto vertexShader = material.GetVertexShader();
	if (vertexShader != nullptr) {
		auto vertexAttributes = vertexShader->GetAttributes();
		uint32 length = static_cast<uint32>(vertexAttributes.size());
		inputElements.resize(length);
		for (uint32 i = 0; i < length; ++i) {
			inputElements[i].SemanticName = ConvertToDXSemantic(vertexAttributes[i].GetInputElementType());
			inputElements[i].SemanticIndex = ConvertToDXSemanticIndex(vertexAttributes[i].GetInputElementType());
			inputElements[i].Format = ConvertToDXFormat(vertexAttributes[i].GetForamt());
			inputElements[i].InputSlot = vertexAttributes[i].GetBinding();
			inputElements[i].AlignedByteOffset = vertexAttributes[i].GetOffset();
			inputElements[i].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			inputElements[i].InstanceDataStepRate = 0;
		}
	}

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc;
	depthStencilDesc.DepthEnable = depthStencil.IsEnableDepthTest();
	depthStencilDesc.DepthWriteMask = depthStencil.IsEnableDepthWrite() ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = ConvertToDXComparisonFunc(depthStencil.GetDepthFunc());

	depthStencilDesc.StencilEnable = depthStencil.IsEnableStencil();

	auto pixelShader = material.GetPixelShader();
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc;
	memset(&pipelineStateDesc, 0, sizeof(pipelineStateDesc));
	pipelineStateDesc.InputLayout = { &inputElements[0], static_cast<uint32>(inputElements.size()) };
	pipelineStateDesc.pRootSignature = rootSignature.Get();
	pipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader->GetHandle());
	pipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader->GetHandle());
	pipelineStateDesc.RasterizerState = rasterizerDesc;
	pipelineStateDesc.BlendState = blendDesc;
	pipelineStateDesc.DepthStencilState = depthStencilDesc;
	pipelineStateDesc.SampleMask = UINT_MAX;
	pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipelineStateDesc.NumRenderTargets = 1;
	pipelineStateDesc.RTVFormats[0] = ConvertToDXFormat(GRAPHICS_FORMAT::R8G8B8A8_UNORM);
	pipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	pipelineStateDesc.SampleDesc.Count = 1;
	auto dxDevice = device->GetDevice();
	HRESULT hr = dxDevice->CreateGraphicsPipelineState(&pipelineStateDesc,
			IID_PPV_ARGS(&pipelineState)
		);
	if (FAILED(hr)) {
		return false;
	}

	viewport = { 0, 0, static_cast<float>(viewportRect.w), static_cast<float>(viewportRect.h), 0.0f, 1.0f, };
	scissor = { 0, 0, static_cast<LONG>(scissorRect.w), static_cast<LONG>(scissorRect.h) };

	return true;
}

void DX12PipelineState::Apply(DX12CommandList& commandList)
{
	auto& dxCommandList = commandList.GetList();
	dxCommandList->SetPipelineState(pipelineState);
	dxCommandList->RSSetViewports(1, &viewport);
	dxCommandList->RSSetScissorRects(1, &scissor);
}
#endif
