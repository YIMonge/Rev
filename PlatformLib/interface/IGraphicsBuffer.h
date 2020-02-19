#ifndef __IGRAPHICSBUFFER_H__
#define __IGRAPHICSBUFFER_H__

#include "revBitset.h"

class IGraphicsBuffer
{
public:
    IGraphicsBuffer(){}
    virtual ~IGraphicsBuffer(){}
    enum GRAPHICS_BUFFER_FORMAT
    {
        GRAPHICS_BUFFER_FORMAT_VERTEX,
        GRAPHICS_BUFFER_FORMAT_NORMAL,
        GRAPHICS_BUFFER_FORMAT_TEXCOORD,
        GRAPHICS_BUFFER_FORMAT_COLOR,
        GRAPHICS_BUFFER_FORMAT_MAX_NUM,
    };
    GRAPHICS_BUFFER_FORMAT getFormat() const
    {
        return format;
    }
protected:
    void SetFormat(GRAPHICS_BUFFER_FORMAT _format)
    {
        format = _format;
    }
private:
    GRAPHICS_BUFFER_FORMAT format;
};

#endif