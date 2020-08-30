#ifdef _USE_DIRECTX12

#include "DX12Texture.h"

DX12Texture::DX12Texture()
{
}

DX12Texture::~DX12Texture()
{
}

bool DX12Texture::Upload(revDevice* device)
{
    // TODO: serialize to meta data
    D3D12_RESOURCE_DESC textureDesc = {};
    textureDesc.MipLevels = 1;
	textureDesc.Format = ConvertToDXFormat(GetFormat()); // DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    textureDesc.DepthOrArraySize = 1;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

    auto dxdevice = device->GetDevice();

    HRESULT hr = dxdevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &textureDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&handle));
    if (FAILED(hr)) return false;

    
    const uint64 bufferSize = GetRequiredIntermediateSize(handle, 0, 1);
    hr = dxdevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&uploadHeap)
        );
    if (FAILED(hr)) {
        handle->Release();
        return false;
    }
    
    D3D12_SUBRESOURCE_DATA textureData = {};
    textureData.pData = rawData;
    textureData.RowPitch = width * 4; // TODO:RGB texture use only 3
    textureData.SlicePitch = textureData.RowPitch * height;

    DX12CommandList& commandList = static_cast<DX12Device*>(device)->GetGlobalCommandList();

    UpdateSubresources(commandList.GetList(), handle, uploadHeap, 0, 0, 1, &textureData);
    commandList.AddTransitionBarrier(handle, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    return true;
}


void DX12Texture::OnUploaded()
{
	if (uploadHeap != nullptr) {
		uploadHeap->Release();
		uploadHeap = nullptr;
	}

	


}


#endif
