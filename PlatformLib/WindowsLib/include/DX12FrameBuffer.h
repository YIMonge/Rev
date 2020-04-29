#ifndef __DX12FRAMEBUFFER_H__
#define __DX12FRAMEBUFFER_H__

#if defined(_USE_DIRECTX12)
#include "DX12SwapChain.h"


class DX12FrameBuffer
{
public:
	DX12FrameBuffer();
	~DX12FrameBuffer();

	bool Create(DX12Device& deviceContext, DX12SwapChain& swapChain);
	void Destroy();


private:
	uint32 handle;
	uint32 descriptorSize;
};

#endif
#endif