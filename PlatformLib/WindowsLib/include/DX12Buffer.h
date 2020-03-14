#ifndef __DX12BUFFER_H__
#define __DX12BUFFER_H__

#ifdef _USE_DIRECTX12
#include "DX12DeviceContext.h"
#include "IGraphicsBuffer.h"
#include "revMath.h"
#include "revArray.h"

class DX12Buffer : public IGraphicsBuffer
{
public:
	DX12Buffer();
	virtual ~DX12Buffer();

	// TODO: set stride for resource view 
	bool Create(const IDeviceContext& deviceContext, const revArray<revVector3>& data, REV_GRAPHICS_BUFFER_FOMAT_FLAG format);
	bool Create(const IDeviceContext& deviceContext, const revArray<revVector4>& data, REV_GRAPHICS_BUFFER_FOMAT_FLAG format);
	bool Create(const IDeviceContext& deviceContext, const revArray<float>& data, REV_GRAPHICS_BUFFER_FOMAT_FLAG format);
	virtual bool Create(const IDeviceContext& deviceContext, const float* data, uint32 size, REV_GRAPHICS_BUFFER_FOMAT_FLAG format);
	void Destroy(const IDeviceContext& deviceContext);

protected:
	ID3D12Resource* buffer;
};

class DX12VertexBuffer : public DX12Buffer
{
public:
	DX12VertexBuffer();
	virtual ~DX12VertexBuffer();

	virtual bool Create(const IDeviceContext& deviceContext, const float* data, uint32 size, REV_GRAPHICS_BUFFER_FOMAT_FLAG format);

	const D3D12_VERTEX_BUFFER_VIEW* GetResourceView() const { return &view; }

private:
	D3D12_VERTEX_BUFFER_VIEW view;
};



#endif
#endif
