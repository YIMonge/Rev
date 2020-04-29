#ifndef __DX12DEVICE_H__
#define __DX12DEVICE_H__

#include "revDevice.h"
#include "revArray.h"
#include "DX12CommandList.h"

#ifdef _USE_DIRECTX12

class Window;

class DX12Device : public revDevice
{
public:
	bool Create(const GraphicsDesc& desc);
	void Destroy();

	DX12CommandList& GetGlobalCommandList() { return globalCommandList; }
	revArray<DX12CommandList>& GetCommandLists() { return commandLists; }
private:
	// commandList for initialize(load asset, pipeline) 
	DX12CommandList globalCommandList;
	// commandList for each thread 
	revArray<DX12CommandList> commandLists;

};

#endif

#endif


