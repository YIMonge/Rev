#ifndef __REVFRAWCOMMAND_H__
#define __REVFRAWCOMMAND_H__

#include "revMatrix44.h"
#include "revColor.h"
#include "revTypedef.h"
#include "IShader.h"
#include "IGraphicsBuffer.h"
#include "revArray.h"

class revMaterial;

// Draw command type
enum DRAW_COMMAND_TYPE
{
    DCT_BEGIN = 0,
    DCT_END,
    DCT_CLEAR_COLOR,
    DCT_CLEAR_DEPTH,
    DCT_CLEAR_STENCIL,
    DCT_MESH_DRAW,
    DCT_MAX_NUM,
};

struct revDrawCommand
{
    DRAW_COMMAND_TYPE           type;               //
    IShader                     program;            //
    revArray<IGraphicsBuffer*>  buffers;            //
    revMaterial*                material;           //
};

#endif /* defined(__REVFRAWCOMMAND_H__) */
