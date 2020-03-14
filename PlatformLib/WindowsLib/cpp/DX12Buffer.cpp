#include "DX12Buffer.h"

#ifdef _USE_DIRECTX12

DX12Buffer::DX12Buffer() :
	buffer(nullptr)
{
}

DX12Buffer::~DX12Buffer()
{
	if (buffer != nullptr) {
		buffer->Release();
	}
}

bool DX12Buffer::Create(const DX12DeviceContext& deviceContext, const revArray<revVector3>& data, GRAPHICS_BUFFER_FORMAT format)
{
	return Create(deviceContext, &data[0].data[0], data.size() * sizeof(revVector3), format);
}

bool DX12Buffer::Create(const DX12DeviceContext& deviceContext, const revArray<revVector4>& data, GRAPHICS_BUFFER_FORMAT format)
{
	return Create(deviceContext, &data[0].data[0], data.size() * sizeof(revVector4), format);
}

bool DX12Buffer::Create(const DX12DeviceContext& deviceContext, const revArray<float>& data, GRAPHICS_BUFFER_FORMAT format)
{
	return Create(deviceContext, &data[0], data.size() * sizeof(float), format);
}

bool DX12Buffer::Create(const DX12DeviceContext& deviceContext, const float* data, uint32 size, GRAPHICS_BUFFER_FORMAT format)
{
	HRESULT hr = deviceContext.GetDevice()->CreateCommittedResource(
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

	CD3DX12_RANGE readRange(0, 0);
	uint8* bufferBegin = nullptr;
	hr = buffer->Map(0, &readRange, reinterpret_cast<void**>(&bufferBegin));
	if (FAILED(hr)) {
		return false;
	}
	memcpy(bufferBegin, data, size);
	buffer->Unmap(0, nullptr);

	return true;
}

void DX12Buffer::Destroy(const DX12DeviceContext& deviceContext)
{
	if (buffer != nullptr) {
		buffer->Release();
		buffer = nullptr;
	}
}


DX12VertexBuffer::DX12VertexBuffer()
{

}

DX12VertexBuffer::~DX12VertexBuffer()
{
}

bool DX12VertexBuffer::Create(const DX12DeviceContext& deviceContext, const float* data, uint32 size, GRAPHICS_BUFFER_FORMAT format)
{
	DX12Buffer::Create(deviceContext, data, size, format);
	view.BufferLocation = buffer->GetGPUVirtualAddress();
	// TODO: set stride 
	view.StrideInBytes = sizeof(revVector4) * 2;
	view.SizeInBytes = size;
	return true;
}


#endif