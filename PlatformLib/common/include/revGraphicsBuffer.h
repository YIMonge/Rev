#ifndef __REVGRAPHICSBUFFER_H__
#define __REVGRAPHICSBUFFER_H__

#include "revMath.h"
#include "revArray.h"
#include "revDevice.h"
#include "revGraphicsResource.h"

class revGraphicsBuffer : public revGraphicsResource
{
public:
    revGraphicsBuffer(revDevice* device) :
		revGraphicsResource(device),
        buffer(NULL_HANDLE),
        sizeOfBytes(0),
        length(0),
        usage(USAGE::STATIC)
        {
		}
    virtual ~revGraphicsBuffer(){}


    enum class USAGE
    {
        STATIC,
        DYNAMIC,
    };

    virtual bool Create(const void* data, uint32 sizeOfBytes, uint32 length, USAGE usage = USAGE::STATIC) = 0;
    virtual void Destroy() = 0;
	virtual bool Update(const void* data, uint32 sizeOfCopyBytes, uint32 offset = 0) = 0;

    const revGraphicsResourceHandle* GetHandlePtr() const { return &buffer; }
    uint32 GetSizeOfBytes() const { return sizeOfBytes; }
    uint32 GetLength() const { return length; }

protected:
    revGraphicsResourceHandle buffer;
    uint32 sizeOfBytes;
    uint32 length;
    USAGE usage;
};

#endif