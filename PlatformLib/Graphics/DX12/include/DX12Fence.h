#ifndef __DX12FENCE_H__
#define __DX12FENCE_H__

#include "DX12Device.h"

class DX12Fence
{
public:
	DX12Fence(DX12Device* device);
	virtual ~DX12Fence();

	bool Create();
	void Destroy();

	void Reset();
	void WaitForQueue();

private:
	DX12Device* device;
	ID3D12Fence* fence;
	uint32 fenceValue;
	HANDLE fenceEvent;
};

#endif
