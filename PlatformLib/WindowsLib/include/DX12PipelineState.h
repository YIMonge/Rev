#ifndef __DX12PIPELINESTATE_H__
#define __DX12PIPELINESTATE_H__

#include "DX12Device.h"
#include "revMaterial.h"
#include "DX12RootSignature.h"

class DX12PipelineState
{
public:
	DX12PipelineState() {}
	virtual ~DX12PipelineState() {}

	bool Create(revDevice* device, const revMaterial& material, const DX12RootSignature& rootSignature);
	void Apply(DX12CommandList& commandList);

	ID3D12DescriptorHeap* GetResourceViewHeap() { return resourceViewHeap; }
private:
	revDevice* device;
	revGraphicsPipeline pipelineState;
	ID3D12DescriptorHeap* resourceViewHeap;
};


#endif
