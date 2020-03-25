#ifndef __REVGRAPHICSBUFFER_H__
#define __REVGRAPHICSBUFFER_H__

#include "revMath.h"
#include "revArray.h"
#include "revDeviceContext.h"

class revGraphicsBuffer
{
public:
    revGraphicsBuffer(){}
    virtual ~revGraphicsBuffer(){}

    virtual bool Create(const revDeviceContext& deviceContext, const revArray<revVector3>& data) = 0;
    virtual bool Create(const revDeviceContext& deviceContext, const revArray<revVector4>& data) = 0;
    virtual bool Create(const revDeviceContext& deviceContext, const revArray<float>& data) = 0;
    virtual bool Create(const revDeviceContext& deviceContext, const float* data, uint32 size) = 0;
    virtual void Destroy(const revDeviceContext& deviceContext) = 0;

    const revGraphicsResource& GetHandle() const { return buffer; }
protected:
    revGraphicsResource buffer;
};

#endif