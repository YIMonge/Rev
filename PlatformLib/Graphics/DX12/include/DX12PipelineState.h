#ifndef __DX12PIPELINESTATE_H__
#define __DX12PIPELINESTATE_H__
#ifdef _USE_DIRECTX12

#include "DX12Device.h"
#include "DX12RootSignature.h"
#include "revMaterial.h"
#include "revRect.h"

class DX12PipelineState
{
public:
	DX12PipelineState() {}
	virtual ~DX12PipelineState() {}

	bool Create(revDevice* device, const revMaterial& material, const DX12RootSignature& rootSignature, const revRect& viewportRect, const revRect& scissorRect);
	void Apply(DX12CommandList& commandList);
private:
	revDevice* device;
	revGraphicsPipeline pipelineState;

	D3D12_VIEWPORT viewport;
	D3D12_RECT scissor;
};


#endif
#endif
