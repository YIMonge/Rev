#ifndef __DX12DEVICECONTEXT_H__
#define __DX12DEVICECONTEXT_H__

#include "../../interface/IDeviceContext.h"

#ifdef _USE_DIRECTX12

class Window;

class DX12DeviceContext : public IDeviceContext
{
public:
	bool Create(const GraphicsDesc& desc);
	void Destroy();

	ID3D12CommandQueue* GetCommandQueue() const
	{
		return commandQueue;
	}

	const GraphicsDesc& GetDesc() const
	{
		return desc;
	}

private:
	ID3D12CommandQueue* commandQueue;
	GraphicsDesc desc;
};

#endif

#endif


