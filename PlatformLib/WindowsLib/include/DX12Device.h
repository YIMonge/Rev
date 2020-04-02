#ifndef __DX12Device_H__
#define __DX12Device_H__

#include "revDevice.h"

#ifdef _USE_DIRECTX12

class Window;

class DX12Device : public revDevice
{
public:
	bool Create(const GraphicsDesc& desc);
	void Destroy();

	bool CreateCommandList(ID3D12PipelineState* pipelineState);
	ID3D12CommandAllocator* GetCommandAllocator() const { return commandAllocator; }
	revGraphicsCommandBuffer GetCommandBuffer() const { return commandBuffer; }


private:
	ID3D12CommandAllocator* commandAllocator;
	revGraphicsCommandBuffer commandBuffer;
};

#endif

#endif


