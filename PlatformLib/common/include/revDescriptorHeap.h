#ifndef __REVDESCRIPTORHEAP_H__
#define __REVDESCRIPTORHEAP_H__

#include "revDevice.h"
#include "revGraphicsCommandList.h"

/// <summary>
/// Abstraction for DescriptorHeap
/// </summary>
class revDescriptorHeap
{
public:
	revDescriptorHeap(revDevice* device) : 
	device(device)
	{}
	virtual ~revDescriptorHeap(){}

	/// <summary>
	/// Creater Descriptor Heap 
	/// </summary>
	/// <param name="type">Descriptor Type</param>
	/// <param name="numDescriptors"></param>
	/// <param name="shaderVisiblity"></param>
	/// <returns></returns>
	virtual bool Create(DESCRIPTOR_HEAP_TYPE type, uint32 numDescriptors, bool shaderVisiblity = true) = 0;	
	/// <summary>
	/// Destroy Descriptor Heap
	/// </summary>
	virtual void Destroy() = 0;
	/// <summary>
	/// Apply Descriptor Heap to CommandList 
	/// </summary>
	/// <param name="commandList">target of command list</param>
	/// <param name="rootIndex">root signature index of descriptor table</param>
	/// <param name="offset"></param>
	virtual void Apply(revGraphicsCommandList& commandList, uint32 rootIndex, uint32 offset = 0) = 0;

	/// <summary>
	/// Infomation of Memory Allocation from DescriptorHeap 
	/// </summary>
	class Chunk
	{
	public:
		Chunk(){}
		virtual uint32 GetAllocationCount() const = 0; 
		virtual revDescriptorCPUHandle GetHandle(uint32 index = 0) const = 0;
	protected:
		revDescriptorCPUHandle handle;
	};

	/// <summary>
	/// Allocation from descriptor heap
	/// </summary>
	/// <param name="allocationNum"></param>
	/// <returns></returns>
	virtual Chunk* Allocation(uint32 allocationNum = 1) = 0;

	/// <summary>
	/// destroy information of allocation 
	/// </summary>
	/// <param name="chunk"></param>
	/// <param name="offset"></param>
	virtual void Free(Chunk* chunk, int32 offset) = 0;
protected:
	revDevice* device;
};

#endif
