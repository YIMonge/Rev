#ifndef __DX12DESCRIPTORHEAP_H__
#define __DX12DESCRIPTORHEAP_H__
#ifdef _USE_DIRECTX12

#include "DX12Device.h"
#include "DX12CommandList.h"
#include "log.h"
#include "revDescriptorHeap.h"

class DX12DescriptorHeap : public revDescriptorHeap
{
public:
	DX12DescriptorHeap(revDevice* device);
	virtual ~DX12DescriptorHeap() {}

	virtual bool Create(DESCRIPTOR_HEAP_TYPE type, uint32 numDescriptors, bool shaderVisiblity = true);
	virtual void Destroy();
	virtual void Apply(revGraphicsCommandList& commandList, uint32 rootIndex, uint32 offset = 0);

	// TODO:refCount
	class Chunk : public revDescriptorHeap::Chunk
	{
		friend class DX12DescriptorHeap;
	public:
		virtual revDescriptorCPUHandle GetHandle(uint32 index = 0) const 
		{
			revDescriptorCPUHandle newHandle = base;
			newHandle.ptr += offsets[index] * incrementalSize;
			return newHandle;
		}

		/// <summary>
		/// get 
		/// </summary>
		/// <returns></returns>
		virtual uint32 GetAllocationCount() const
		{
			return static_cast<uint32>(offsets.size());
		}

		/// <summary>
		/// get descriptor heap offset
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		uint32 GetOffset(uint32 index)
		{
			return offsets[index];
		}

	private:
		Chunk(revDescriptorCPUHandle handle, uint32 incrementalSize) {
			base = handle;
			this->incrementalSize = incrementalSize;
		}
	private:
		revDescriptorCPUHandle base;
		revArray<uint32> offsets;
		uint32 incrementalSize;
	};

	virtual Chunk* Allocation(uint32 allocationNum = 1) 
	{ 
		// TODO: allocNUm + allocationNum < maxDescriptors  Check 
		Chunk* chunk = new Chunk(cpuHandle, incrementalSize);
		chunk->offsets.resize(allocationNum);
		for (uint32 i = 0; i < allocationNum; ++i)
		{
			chunk->offsets[i] = availableInfo->Use();
		}
		allocNum += allocationNum;
		return chunk;
	}
		 
	virtual void Free(revDescriptorHeap::Chunk* chunk, int32 offset = -1)
	{
		if (chunk == nullptr) return;
		Chunk* dx12DescriptorHeapChunk = (Chunk*)chunk;
		// release a part of chunk 
		if (offset != -1) {
			FreeSingleHandle(dx12DescriptorHeapChunk, offset);
			return;
		}
		uint32 count = static_cast<uint32>(dx12DescriptorHeapChunk->offsets.size());
		for (uint32 i = 0; i < count; ++i) {
			FreeSingleHandle(dx12DescriptorHeapChunk, i);
		}
	}

	class AvailableDescriptorHeapInfo
	{
	public:
		AvailableDescriptorHeapInfo(uint32 size)
		{
			if (size == UINT32_MAX) {
				NATIVE_LOGE("Max Descriptor size over!!!");
				return;
			}
			this->size = size;
			uint32 index = size / sizeof(uint32);
			uint32 allocationSize = index == 0 ? 1 : index;
			buffer = new uint32[allocationSize];
			memset(buffer, 0, allocationSize * sizeof(uint32));
		}

		~AvailableDescriptorHeapInfo()
		{
			delete[] buffer;
		}

		/// <summary>
		/// use block if heap has empty block.
		/// </summary>
		/// <returns>used block no. it is neede when release block</returns>
		uint32 Use()
		{
			uint32 index = 0;
			uint32 tempMaxSize = size / sizeof(uint32);
			const uint32 MAX_PAGE_NUM = tempMaxSize == 0 ? 1 : tempMaxSize;
			for (; index < MAX_PAGE_NUM; ++index) {
				if (buffer[index] < UINT32_MAX) break;
			}

			if (index == MAX_PAGE_NUM) return UINT32_MAX;

			uint32 bitNo = 0;
			for (; bitNo < 32; ++bitNo) {
				if ((buffer[index] & (1 << bitNo)) == 0) {
					break;
				}
			}

			buffer[index] |= (1 << bitNo);
			return index * sizeof(uint32) + bitNo;
		}

		/// <summary>
		/// release block 
		/// </summary>
		/// <param name="no"></param>
		void Release(uint32 no)
		{
			if (size <= no) {
				NATIVE_LOGE("cant free descriptor heap. buffer over");
				return;
			}
			uint32 index = no / sizeof(uint32);
			if ((buffer[index] & (1 << (no % sizeof(uint32)))) == 0) {
				NATIVE_LOGE("cant free descriptor heap. already free");
				return;
			}

			buffer[index] &= (1 << (no % sizeof(uint32)));
		}

	private:
		uint32 size;
		uint32* buffer;
	};

private:
	template<typename T>
	T GetHandle(T handle, uint32 index)
	{
		T newHandle = handle;
		newHandle.ptr += index * incrementalSize;
		return newHandle;
	}

	void FreeSingleHandle(Chunk* chunk, int32 offset)
	{
		if (chunk == nullptr) return;
		uint32 bitNo = chunk->offsets[offset];
		availableInfo->Release(bitNo);
		--allocNum;
	}



	DESCRIPTOR_HEAP_TYPE type;
	uint32 maxDescriptors;
	uint32 incrementalSize;
	uint32 allocNum;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	ID3D12DescriptorHeap* heap;
	AvailableDescriptorHeapInfo* availableInfo;
};

#endif
#endif