#ifndef __DX12DEVICECONTEXT_H__
#define __DX12DEVICECONTEXT_H__

#include "revDeviceContext.h"

#ifdef _USE_DIRECTX12

class Window;

class DX12DeviceContext : public revDeviceContext
{
public:
	bool Create(const GraphicsDesc& desc);
	void Destroy();

	bool CreateCommandList(ID3D12PipelineState* pipelineState);
	ID3D12CommandAllocator* GetCommandAllocator() const { return commandAllocator; }
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList; }

private:
	ID3D12CommandAllocator* commandAllocator;
	ID3D12GraphicsCommandList* commandList;

};

#endif

#endif


