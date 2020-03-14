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

	bool Create(const DX12DeviceContext& deviceContext, const revArray<revVector3>& data, GRAPHICS_BUFFER_FORMAT format);
	bool Create(const DX12DeviceContext& deviceContext, const revArray<revVector4>& data, GRAPHICS_BUFFER_FORMAT format);
	bool Create(const DX12DeviceContext& deviceContext, const revArray<float>& data, GRAPHICS_BUFFER_FORMAT format);
	virtual bool Create(const DX12DeviceContext& deviceContext, const float* data, uint32 size, GRAPHICS_BUFFER_FORMAT format);
	void Destroy(const DX12DeviceContext& deviceContext);

protected:
	ID3D12Resource* buffer;
};

class DX12VertexBuffer : public DX12Buffer
{
public:
	DX12VertexBuffer();
	virtual ~DX12VertexBuffer();

	virtual bool Create(const DX12DeviceContext& deviceContext, const float* data, uint32 size, GRAPHICS_BUFFER_FORMAT format);

	const D3D12_VERTEX_BUFFER_VIEW* GetResourceView() const { return &view; }

private:
	D3D12_VERTEX_BUFFER_VIEW view;
};



#endif
#endif
