#ifndef __REVGRAPHICSBUFFER_H__
#define __REVGRAPHICSBUFFER_H__

#include "revMath.h"
#include "revArray.h"
#include "revDevice.h"

class revGraphicsBuffer
{
public:
    revGraphicsBuffer(revDevice* device) :
        buffer(NULL_HANDLE),
        sizeOfBytes(0),
        length(0),
        usage(USAGE::STATIC)
        {
			this->device = device;
		}
    virtual ~revGraphicsBuffer(){}


    enum class USAGE
    {
        STATIC,
        DYNAMIC,
    };

    virtual bool Create(const void* data, uint32 sizeOfBytes, uint32 length, USAGE usage = USAGE::STATIC) = 0;
    virtual void Destroy() = 0;

    const revGraphicsResource* GetHandlePtr() const { return &buffer; }
    uint32 GetSizeOfBytes() const { return sizeOfBytes; }
    uint32 GetLength() const { return length; }

protected:
    revDevice* device;
    revGraphicsResource buffer;
    uint32 sizeOfBytes;
    uint32 length;
    USAGE usage;
};

#endif