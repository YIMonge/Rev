#ifndef __DX12BUFFER_H__
#define __DX12BUFFER_H__

#ifdef _USE_DIRECTX12
#include "DX12Device.h"
#include "revGraphicsBuffer.h"
#include "revMath.h"
#include "revArray.h"

class DX12Buffer : public revGraphicsBuffer
{
public:
	DX12Buffer();
	virtual ~DX12Buffer();

	// TODO: set stride for resource view 
	bool Create(const revDevice& deviceContext, const revArray<revVector3>& data);
	bool Create(const revDevice& deviceContext, const revArray<revVector4>& data);
	bool Create(const revDevice& deviceContext, const revArray<float>& data);
	virtual bool Create(const revDevice& deviceContext, const float* data, uint32 size);
	void Destroy(const revDevice& deviceContext);

protected:
	ID3D12Resource* buffer;
};

class DX12VertexBuffer : public DX12Buffer
{
public:
	DX12VertexBuffer();
	virtual ~DX12VertexBuffer();

	virtual bool Create(const revDevice& deviceContext, const float* data, uint32 size);

	const D3D12_VERTEX_BUFFER_VIEW* GetResourceView() const { return &view; }

private:
	D3D12_VERTEX_BUFFER_VIEW view;
};



#endif
#endif
