#ifndef __DX12RENDERINFO_H__
#define __DX12RENDERINFO_H__

#include "DX12DeviceContext.h"
#include "DX12Shader.h"
#include "revMaterial.h"

class DX12RenderInfo
{
public:
	DX12RenderInfo();
	~DX12RenderInfo();

	bool CreateSignature(const DX12DeviceContext& deviceContext);


	bool CreatePipeline(const DX12DeviceContext& deviceContext, const DX12Shader& vertexShader, const DX12Shader& fragmentShader);
	bool CreatePipeline(const DX12DeviceContext& deviceContext, const revMaterial& material);

	ID3D12PipelineState* GetPipelineState() const {	return pipelineState; }
	ID3D12RootSignature* GetRootSignature() const {	return rootSignature; }
	ID3D12DescriptorHeap* GetResourceViewHeap() const { return resourceViewHeap; }

private:
	ID3D12RootSignature* rootSignature;
	ID3D12PipelineState* pipelineState;

	ID3D12DescriptorHeap* resourceViewHeap;
};

#endif
