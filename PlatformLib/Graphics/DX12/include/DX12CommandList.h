#ifndef __DX12COMMANDLIST_H__
#define __DX12COMMANDLIST_H__
#ifdef _USE_DIRECTX12

#include "revGraphicsCommandList.h"
#include "revArray.h"


class DX12CommandList : public revGraphicsCommandList
{
public:
	DX12CommandList(){}
	virtual ~DX12CommandList(){}

	virtual bool Create(revDevice* device, revGraphicsPipeline* pipeline, revGraphicsCommandAllocator* allocator = nullptr);
	virtual void Open();
	virtual void Close();
	void SetPipelineState(revGraphicsPipeline& pipeline);

	void AddTransitionBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after, uint32 subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);
	void ReserveRelease(ID3D12Resource* resource) {	needReleaseResources.push_back(resource); }
	void ReleaseResoucers();

private:
	revArray<D3D12_RESOURCE_BARRIER> needBarrierResources;
	revArray<ID3D12Resource*> needReleaseResources;

};

#endif
#endif
