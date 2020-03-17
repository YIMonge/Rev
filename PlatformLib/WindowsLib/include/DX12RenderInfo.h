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

	bool Create(const DX12DeviceContext& deviceContext, const DX12Shader& vertexShader, const DX12Shader& fragmentShader);

	ID3D12PipelineState* GetPipelineState() const 
	{
		return pipelineState;
	}
	ID3D12RootSignature* GetRootSignature() const
	{
		return rootSignature;
	}


private:
	ID3D12RootSignature* rootSignature;
	ID3D12PipelineState* pipelineState;
};

#endif
