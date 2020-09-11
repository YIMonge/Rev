
#ifdef _USE_DIRECTX12

#include "DX12DescriptorHeap.h"
#include "Log.h"

DX12DescriptorHeap::DX12DescriptorHeap(revDevice* device) :
revDescriptorHeap(device),
heap(nullptr),
type(DESCRIPTOR_HEAP_TYPE::TEXTURE),
maxDescriptors(0),
allocNum(0),
incrementalSize(0)
{
}

bool DX12DescriptorHeap::Create(DESCRIPTOR_HEAP_TYPE type, uint32 numDescriptors, bool shaderVisiblity)
{
	this->type = type;
	this->maxDescriptors = numDescriptors;
	availableInfo = new AvailableDescriptorHeapInfo(numDescriptors);
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
	if (availableInfo != nullptr) delete availableInfo;
	if (heap != nullptr) heap->Release();
}

void DX12DescriptorHeap::Apply(revGraphicsCommandList& commandList, uint32 rootIndex, uint32 offset)
{
	auto& list = commandList.GetList();
	list->SetDescriptorHeaps(1, &heap);
	CD3DX12_GPU_DESCRIPTOR_HANDLE descriptorHandle(gpuHandle, offset, incrementalSize);
	list->SetGraphicsRootDescriptorTable(rootIndex, descriptorHandle);
}

#endif
