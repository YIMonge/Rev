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
	DX12Buffer(revDevice* device);
	virtual ~DX12Buffer();

	// TODO: set stride for resource view 
	virtual bool Create(const revArray<revVector3>& data, USAGE usage = USAGE::STATIC);
	virtual bool Create(const revArray<revVector4>& data, USAGE usage = USAGE::STATIC);
	virtual bool Create(const revArray<float>& data, USAGE usage = USAGE::STATIC);
	virtual bool Create(const float* data, uint32 sizeOfBytes, uint32 length, USAGE usage = USAGE::STATIC);
	virtual void Destroy();

	bool Update(const float* data, uint32 sizeOfCopyBytes, uint32 offset = 0);
private:
	void* mappedMemory;
};

class DX12VertexBuffer : public DX12Buffer
{
public:
	DX12VertexBuffer(revDevice* device) : 
		DX12Buffer(device)
	{}
	virtual ~DX12VertexBuffer(){}

	virtual bool Create(const float* data, uint32 sizeOfBytes, uint32 length, USAGE usage = USAGE::STATIC);
};


class DX12ConstantBuffer : public DX12Buffer
{
public:
	DX12ConstantBuffer(revDevice* device) :
		DX12Buffer(device)
	{}
	virtual ~DX12ConstantBuffer(){}

	virtual bool Create(const float* data, uint32 sizeOfBytes, uint32 length, USAGE usage = USAGE::STATIC);

	const D3D12_CONSTANT_BUFFER_VIEW_DESC* GetResourceViewDesc() const { return &view; }
private:
	D3D12_CONSTANT_BUFFER_VIEW_DESC view;
};

#endif
#endif
