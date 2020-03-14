#ifndef __IGRAPHICSBUFFER_H__
#define __IGRAPHICSBUFFER_H__

#include "revMath.h"
#include "revArray.h"
#include "IDeviceContext.h"

class IGraphicsBuffer
{
public:
    IGraphicsBuffer(){}
    virtual ~IGraphicsBuffer(){}

    virtual bool Create(const IDeviceContext& deviceContext, const revArray<revVector3>& data, REV_GRAPHICS_BUFFER_FOMAT_FLAG format) = 0;
    virtual bool Create(const IDeviceContext& deviceContext, const revArray<revVector4>& data, REV_GRAPHICS_BUFFER_FOMAT_FLAG format) = 0;
    virtual bool Create(const IDeviceContext& deviceContext, const revArray<float>& data, REV_GRAPHICS_BUFFER_FOMAT_FLAG format) = 0;
    virtual bool Create(const IDeviceContext& deviceContext, const float* data, uint32 size, REV_GRAPHICS_BUFFER_FOMAT_FLAG format) = 0;
    virtual void Destroy(const IDeviceContext& deviceContext) = 0;

    REV_GRAPHICS_BUFFER_FOMAT_FLAG getFormat() const { return format;}
    const revGraphicsResource& GetHandle() const { return buffer; }

protected:
    REV_GRAPHICS_BUFFER_FOMAT_FLAG format;
    revGraphicsResource buffer;
};

#endif