#ifndef __DX12DEVICE_H__
#define __DX12DEVICE_H__
#ifdef _USE_DIRECTX12

#include "revDevice.h"
#include "revArray.h"
#include "DX12CommandList.h"

class Window;

class DX12Device : public revDevice
{
public:
	bool Create(const GraphicsDesc& desc);
	void Destroy();

	DX12CommandList& GetGlobalCommandList() { return globalCommandList; }
	revArray<DX12CommandList>& GetCommandLists() { return commandLists; }
	IDXGIFactory4* GetFactory() const { return dxgiFactory; }

private:
	// commandList for initialize(load asset, pipeline) 
	DX12CommandList globalCommandList;
	// commandList for each thread 
	revArray<DX12CommandList> commandLists;
	// DXGIFactory
	IDXGIFactory4* dxgiFactory;

};

#endif

#endif


