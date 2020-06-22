#ifdef _USE_DIRECTX12

#include "DX12Buffer.h"

DX12Buffer::DX12Buffer() :
	mappedMemory(nullptr)
{
	buffer = nullptr;
}

DX12Buffer::~DX12Buffer()
{
	if (buffer != nullptr) {
		buffer->Release();
	}
}

bool DX12Buffer::Create(revDevice* device, const revArray<revVector3>& data, USAGE usage)
{
	return Create(device, &data[0].data[0], sizeof(revVector3), static_cast<uint32>(data.size()), usage);
}

bool DX12Buffer::Create(revDevice* device, const revArray<revVector4>& data, USAGE usage)
{
	return Create(device, &data[0].data[0], sizeof(revVector4), static_cast<uint32>(data.size()), usage);
}

bool DX12Buffer::Create(revDevice* device, const revArray<float>& data, USAGE usage)
{
	return Create(device, &data[0], sizeof(float), static_cast<uint32>(data.size()), usage);
}

bool DX12Buffer::Create(revDevice* device, const float* data, uint32 sizeOfBytes, uint32 length, USAGE usage)
{
	this->usage = usage;
	this->device = device;
	this->length = length;
	this->sizeOfBytes = sizeOfBytes;
	uint32 size = sizeOfBytes * length;
	HRESULT hr = device->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(size),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buffer)
	);
	if (FAILED(hr)) {
		return false;
	}

	return Update(data, size);
}

bool DX12Buffer::Update(const float* data, uint32 sizeOfCopyBytes, uint32 offset)
{
	if (data == nullptr) return false;

	if (mappedMemory == nullptr) {
		CD3DX12_RANGE readRange(0, 0);
		HRESULT hr = buffer->Map(0, &readRange, &mappedMemory);
		if (FAILED(hr)) {
			return false;
		}
	}
	memcpy(mappedMemory, data+offset, sizeOfCopyBytes);
	if (usage == USAGE::STATIC) {
		buffer->Unmap(0, nullptr);
		mappedMemory = nullptr;
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

bool DX12VertexBuffer::Create(revDevice* device, const float* data, uint32 sizeOfBytes, uint32 length, USAGE usage)
{
	return DX12Buffer::Create(device, data, sizeOfBytes, length);
}


bool DX12ConstantBuffer::Create(revDevice* device, const float* data, uint32 sizeOfBytes, uint32 length, USAGE usage)
{
	return DX12Buffer::Create(device, data, sizeOfBytes, length);
}


#endif