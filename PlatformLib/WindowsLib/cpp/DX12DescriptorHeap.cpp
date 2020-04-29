#include "DX12DescriptorHeap.h"
#include "Log.h"

DX12DescriptorHeap::DX12DescriptorHeap() :
device(nullptr),
heap(nullptr),
type(DESCRIPTOR_HEAP_TYPE::RESOURCE),
maxDescriptors(0),
allocNum(0),
incrementalSize(0)
{
}

bool DX12DescriptorHeap::Create(DX12Device* device, DESCRIPTOR_HEAP_TYPE type, uint32 numDescriptors, bool shaderVisiblity)
{
	this->device = device;
	this->type = type;
	this->maxDescriptors = numDescriptors;
	auto& dxDevice = device->GetDevice();

	D3D12_DESCRIPTOR_HEAP_DESC  descriptorHeapDesc = {};
	descriptorHeapDesc.NumDescriptors = numDescriptors;
	descriptorHeapDesc.Type = ConvertToDXDescriptorHeapType(type);
	descriptorHeapDesc.Flags = shaderVisiblity ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT hr = dxDevice->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&heap));
	if (FAILED(hr)) {
		NATIVE_LOGE("could not create to descriptor heap. File[%s], line[%d]", __FILE__, __LINE__);
		return false;
	}

	cpuHandle = heap->GetCPUDescriptorHandleForHeapStart();
	gpuHandle = heap->GetGPUDescriptorHandleForHeapStart();

	incrementalSize = dxDevice->GetDescriptorHandleIncrementSize(descriptorHeapDesc.Type);
	return true;
}

void DX12DescriptorHeap::Destroy()
{
	if (heap != nullptr) heap->Release();
}

void DX12DescriptorHeap::Apply(DX12CommandList& commandList, uint32 rootIndex)
{
	auto& list = commandList.GetList();
	list->SetDescriptorHeaps(1, &heap);
	list->SetGraphicsRootDescriptorTable(rootIndex, gpuHandle);
}
