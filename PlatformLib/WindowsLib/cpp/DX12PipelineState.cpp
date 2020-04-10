#include "DX12PipelineState.h"

const char* ConvertToDXSemantic(INPUT_ELEMENT_TYPE type)
{
	const char* table[] = {
		"POSITION",
		"NORMAL",
		"TANGENT",
		"TEXCOORD0",
		"TEXCOORD1",
		"TEXCOORD2",
		"TEXCOORD3",
		"COLOR0",
		"COLOR1",
		"COLOR2",
		"COLOR3",
	};
	return table[static_cast<uint32>(type)];
}

uint32 ConvertToDXSemanticIndex(INPUT_ELEMENT_TYPE type)
{
	uint32 table[] = {
		0, // "POSITION"
		0, // "NORMAL"
		0, // "TANGENT"
		0, // "TEXCOORD0"
		1, // "TEXCOORD1"
		2, // "TEXCOORD2"
		3, // "TEXCOORD3"
		0, // "COLOR0"
		1, // "COLOR1"
		2, // "COLOR2"
		3, // "COLOR3"
	};
	return table[static_cast<uint32>(type)];
}


bool DX12PipelnieState::Create(revDevice* device, const revMaterial& material, const DX12RootSignature& rootSignature)
{
	auto blendState = material.GetBlendState();
	auto rasterizerState = material.GetRasterization();

	D3D12_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.FillMode = ConvertToDXFillMode(rasterizerState.GetPolygonMode());
	rasterizerDesc.CullMode = ConvertToDXCullMode(rasterizerState.GetCullMode());
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
		auto vertexAttributes = vertexShader->GetMetaData().GetAttributes();
		uint32 length = vertexAttributes.size();
		for (uint32 i = 0; i < length; ++i) {
			inputElements[i].SemanticName = ConvertToDXSemantic(vertexAttributes[i].GetInputElementType());
			inputElements[i].SemanticIndex = ConvertToDXSemanticIndex(vertexAttributes[i].GetInputElementType());
			inputElements[i].Format = ConvertToDXFormat(vertexAttributes[i].GetForamt());
			inputElements[i].InputSlot = vertexAttributes[i].GetBinding();
			inputElements[i].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
			inputElements[i].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			inputElements[i].InstanceDataStepRate = 0;
		}
	}

	auto pixelShader = material.GetPixelShader();

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc;
	memset(&pipelineStateDesc, 0, sizeof(pipelineStateDesc));
	pipelineStateDesc.InputLayout = { &inputElements[0], inputElements.size() };
	pipelineStateDesc.pRootSignature = rootSignature.Get();
	pipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader->GetHandle());
	pipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader->GetHandle());
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

	return true;
}