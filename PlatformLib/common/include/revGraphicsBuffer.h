#ifndef __REVGRAPHICSBUFFER_H__
#define __REVGRAPHICSBUFFER_H__

#include "revMath.h"
#include "revArray.h"
#include "revDevice.h"

class revGraphicsBuffer
{
public:
    revGraphicsBuffer():
        buffer(NULL_HANDLE),
        length(0)
        {}
    virtual ~revGraphicsBuffer(){}

    virtual bool Create(const revDevice& deviceContext, const revArray<revVector3>& data) = 0;
    virtual bool Create(const revDevice& deviceContext, const revArray<revVector4>& data) = 0;
    virtual bool Create(const revDevice& deviceContext, const revArray<float>& data) = 0;
    virtual bool Create(const revDevice& deviceContext, const float* data, uint32 sizeOfBytes, uint32 length) = 0;
    virtual void Destroy(const revDevice& deviceContext) = 0;

    const revGraphicsResource& GetHandle() const { return buffer; }
protected:
    revGraphicsResource buffer;
    uint32 length;
};

#endif