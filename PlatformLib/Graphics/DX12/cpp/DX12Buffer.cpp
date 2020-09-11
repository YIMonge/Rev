#ifdef _USE_DIRECTX12

#include "DX12Buffer.h"
#include "DX12VertexBufferView.h"
#include "DX12IndexBufferView.h"
#include "DX12ConstantBufferView.h"

DX12Buffer::DX12Buffer(revDevice* device) :
	revGraphicsBuffer(device),
	mappedMemory(nullptr)
{
	buffer = nullptr;
	destResourceState = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
}

DX12Buffer::~DX12Buffer()
{
	if (buffer != nullptr) {
		buffer->Release();
	}
}

bool DX12Buffer::Create(const void* data, uint32 sizeOfBytes, uint32 length, USAGE usage)
{
	this->usage = usage;
	this->length = length;
	this->sizeOfBytes = sizeOfBytes;
	uint32 size = sizeOfBytes * length;

	D3D12_HEAP_TYPE heapType = D3D12_HEAP_TYPE_DEFAULT;
	D3D12_RESOURCE_STATES resourceState = D3D12_RESOURCE_STATE_COPY_DEST;
	if (usage == USAGE::DYNAMIC) {
		heapType = D3D12_HEAP_TYPE_UPLOAD;
		resourceState = D3D12_RESOURCE_STATE_GENERIC_READ;
	}


	HRESULT hr = device->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(heapType),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(size),
		resourceState,
		nullptr,
		IID_PPV_ARGS(&buffer)
	);
	if (FAILED(hr)) {
		return false;
	}

	return Update(data, size);
}

bool DX12Buffer::Update(const void* data, uint32 sizeOfCopyBytes, uint32 offset)
{
	if (data == nullptr) return false;

	if (usage == USAGE::DYNAMIC) {
		if (mappedMemory == nullptr) {
			CD3DX12_RANGE readRange(0, 0);
			HRESULT hr = buffer->Map(0, &readRange, &mappedMemory);
			if (FAILED(hr)) {
				return false;
			}
		}
		memcpy(mappedMemory, (uint32*)data + offset, sizeOfCopyBytes);
		buffer->Unmap(0, nullptr);
		mappedMemory = nullptr;
	}
	else {
		subResourceData = {};
		subResourceData.pData = data;
		subResourceData.RowPitch = sizeOfCopyBytes;
		subResourceData.SlicePitch = subResourceData.RowPitch;

	}
	return true;
}


void DX12Buffer::Destroy()
{
	if (usage == USAGE::STATIC && mappedMemory != nullptr) {
		buffer->Unmap(0, nullptr);
		mappedMemory = nullptr;
	}

	if (buffer != nullptr) {
		buffer->Release();
		buffer = nullptr;
	}
}

bool DX12Buffer::Upload(revDevice* device)
{
	if (usage == USAGE::STATIC) {
		HRESULT hr = device->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(subResourceData.RowPitch),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&uploadBuffer)
		);
		if (FAILED(hr)) {
			return false;
		}

		DX12CommandList& commandList = static_cast<DX12Device*>(device)->GetGlobalCommandList();
		UpdateSubresources(commandList.GetList(), buffer, uploadBuffer, 0, 0, 1, &subResourceData);
		commandList.AddTransitionBarrier(buffer, D3D12_RESOURCE_STATE_COPY_DEST, destResourceState);
	}
	return true;
}

void DX12Buffer::ReleaseUploadBuffer()
{
	if (usage == USAGE::STATIC && uploadBuffer != nullptr) {
		uploadBuffer->Release();
		uploadBuffer = nullptr;
	}
}

bool DX12VertexBuffer::Create(const revArray<revVector3>& data, USAGE usage)
{
	return DX12Buffer::Create(&data[0].data[0], sizeof(revVector3), static_cast<uint32>(data.size()), usage);
}

bool DX12VertexBuffer::Create(const revArray<revVector4>& data, USAGE usage)
{
	return DX12Buffer::Create(&data[0].data[0], sizeof(revVector4), static_cast<uint32>(data.size()), usage);
}

bool DX12VertexBuffer::Create(const revArray<float>& data, USAGE usage)
{
	return DX12Buffer::Create(&data[0], sizeof(float), static_cast<uint32>(data.size()), usage);
}

revGraphicsResource* DX12VertexBuffer::OnUploaded(revDevice* device)
{
	DX12VertexBufferView* view = new DX12VertexBufferView(device);
	view->Create(this);
	return view;
}

bool DX12IndexBuffer::Create(const uint32* index, uint32 length)
{
	return DX12Buffer::Create(index, sizeof(revIndex3), length, USAGE::STATIC);
}

revGraphicsResource* DX12IndexBuffer::OnUploaded(revDevice* device)
{
	DX12IndexBufferView* view = new DX12IndexBufferView(device);
	view->Create(this);
	return view;
}

revGraphicsResource* DX12ConstantBuffer::OnUploaded(revDevice* device)
{
	ReleaseUploadBuffer();

	DX12ConstantBufferView* view = new DX12ConstantBufferView(device);
	revDescriptorHeap* heap = device->GetDescriptorHeap(DESCRIPTOR_HEAP_TYPE::BUFFER);
	revDescriptorHeap::Chunk* chunk = heap->Allocation();
	view->Create(this, heap, chunk);
	return view;
}


#endif