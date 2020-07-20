#ifndef __DX12DESCRIPTORHEAP_H__
#define __DX12DESCRIPTORHEAP_H__
#ifdef _USE_DIRECTX12

#include "DX12Device.h"
#include "DX12CommandList.h"
#include "log.h"

class DX12DescriptorHeap 
{
public:
	DX12DescriptorHeap();
	virtual ~DX12DescriptorHeap() {}

	bool Create(DX12Device* device, DESCRIPTOR_HEAP_TYPE type, uint32 numDescriptors, bool shaderVisiblity = true);
	void Destroy();

	void Apply(revGraphicsCommandList& commandList, uint32 rootIndex, uint32 offset = 0);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint32 index = 0) 
	{
		if (allocNum < index) NATIVE_LOGE("failed to get handle from descriptor heap. File[%s], line[%d]", __FILE__, __LINE__);
		return GetHandle(cpuHandle, index); 
	}
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(uint32 index = 0) 
	{ 
		if (allocNum < index) NATIVE_LOGE("failed to get handle from descriptor heap. File[%s], line[%d]", __FILE__, __LINE__);
		return GetHandle(gpuHandle, index); 
	}

	class Chunk
	{
		friend class DX12DescriptorHeap;
	public:
		Chunk(){}

		D3D12_CPU_DESCRIPTOR_HANDLE GetHandle(uint32 index = 0)
		{
			D3D12_CPU_DESCRIPTOR_HANDLE newHandle = base;
			if (index >= maxAllocationNum) {
				NATIVE_LOGE("Descriptor Heap Over run! file:%s, line:%s", __FILE__, __LINE__);
				newHandle.ptr = 0;
				return newHandle;
			}
			newHandle.ptr += (index+startIndex) * incrementalSize;
			return newHandle;
		}

		uint32 GetIndexStartAllocation() const { return startIndex; }
		uint32 GetDescriptorOffset(uint32 indexOfAllocation) const { return startIndex + indexOfAllocation; }
	private:
		Chunk(D3D12_CPU_DESCRIPTOR_HANDLE handle, uint32 allocationNum, uint32 startIndex, uint32 incrementalSize) {
			base = handle;
			maxAllocationNum = allocationNum;
			this->startIndex = startIndex;
			this->incrementalSize = incrementalSize;
		}


	private:
		D3D12_CPU_DESCRIPTOR_HANDLE base;
		uint32 maxAllocationNum;
		uint32 incrementalSize;
		uint32 startIndex;
	};

	Chunk Allocation(uint32 allocationNum) 
	{ 
		// TODO: allocNUm + allocationNum < maxDescriptors  Check 
		Chunk chunk(cpuHandle, allocationNum, allocNum, incrementalSize);
		allocNum += allocationNum;
		return chunk;
	}
		 

private:
	template<typename T>
	T GetHandle(T handle, uint32 index)
	{
		T newHandle = handle;
		newHandle.ptr += index * incrementalSize;
		return newHandle;
	}

	DX12Device* device;
	DESCRIPTOR_HEAP_TYPE type;
	uint32 maxDescriptors;
	uint32 incrementalSize;
	uint32 allocNum;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	revDescriptorHeap* heap;
};

#endif
#endif