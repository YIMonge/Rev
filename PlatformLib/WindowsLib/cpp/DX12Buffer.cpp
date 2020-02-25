#include "DX12Buffer.h"

#if defined(_USE_DIRECTX12)

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

}

bool DX12Buffer::Create(const DX12DeviceContext& deviceContext, const revArray<revVector4>& data, GRAPHICS_BUFFER_FORMAT format)
{

}

bool DX12Buffer::Create(const DX12DeviceContext& deviceContext, const revArray<float>& data, GRAPHICS_BUFFER_FORMAT format)
{

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
	//hr = buffer->Map(0, readRange);

	return true;
}

void DX12Buffer::Destroy(const DX12DeviceContext& deviceContext)
{
	if (buffer != nullptr) {
		buffer->Release();
		buffer = nullptr;
	}
}

#endif