#ifndef __DX12RENDERINFO_H__
#define __DX12RENDERINFO_H__

#include "DX12Device.h"
#include "DX12Shader.h"
#include "revMaterial.h"

class DX12RenderInfo
{
public:
	DX12RenderInfo();
	~DX12RenderInfo();

	bool CreatePipeline(const DX12Device& deviceContext, const DX12Shader& vertexShader, const DX12Shader& fragmentShader);
	bool CreatePipeline(const DX12Device& deviceContext, const revMaterial& material);

private:
	ID3D12PipelineState* pipelineState;
	ID3D12DescriptorHeap* resourceViewHeap;
};

#endif
