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

	virtual bool Create(const void* data, uint32 sizeOfBytes, uint32 length, USAGE usage = USAGE::STATIC);
	virtual void Destroy();
	virtual bool Update(const void* data, uint32 sizeOfCopyBytes, uint32 offset = 0);
protected:
	D3D12_RESOURCE_STATES destResourceState;

private:
	void* mappedMemory;
	revGraphicsResource uploadBuffer;
};

class DX12VertexBuffer : public DX12Buffer
{
public:
	DX12VertexBuffer(revDevice* device) : 
		DX12Buffer(device)
	{}
	virtual ~DX12VertexBuffer(){}

	bool Create(const revArray<revVector3>& data, USAGE usage = USAGE::STATIC);
	bool Create(const revArray<revVector4>& data, USAGE usage = USAGE::STATIC);
	bool Create(const revArray<float>& data, USAGE usage = USAGE::STATIC);
};


class DX12ConstantBuffer : public DX12Buffer
{
public:
	DX12ConstantBuffer(revDevice* device) :
		DX12Buffer(device)
	{}
	virtual ~DX12ConstantBuffer(){}
};

class DX12IndexBuffer : public DX12Buffer
{
public:
	DX12IndexBuffer(revDevice* device) :
		DX12Buffer(device) 
	{
		destResourceState = D3D12_RESOURCE_STATE_INDEX_BUFFER;
	}
	virtual ~DX12IndexBuffer() {}

	bool Create(const uint32* index, uint32 length);
};

#endif
#endif
