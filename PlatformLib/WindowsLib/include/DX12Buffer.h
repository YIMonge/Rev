#ifndef __DX12BUFFER_H__
#define __DX12BUFFER_H__

#if defined(_USE_DIRECTX12)
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
	bool Create(const DX12DeviceContext& deviceContext, const float* data, uint32 size, GRAPHICS_BUFFER_FORMAT format);
	void Destroy(const DX12DeviceContext& deviceContext);

private:
	ID3D12Resource* buffer;
};




#endif
#endif
