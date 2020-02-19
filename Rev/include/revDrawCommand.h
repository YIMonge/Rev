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
    union {
        DRAW_COMMAND_TYPE type;                     // type of draw command
        uint32 next;                                // next index for storage
    };
    IShader*                    program;            // shader
    revArray<IGraphicsBuffer*>  buffers;            // shader resources
    revMaterial*                material;           // material
};

// DrawCommand cache, if use multi thread must set threadNum for lock free.
class revDrawCommandStorage
{
public:
    revDrawCommandStorage(uint32 capacity = 1024, uint32 threadNum = 1);
    ~revDrawCommandStorage();

    revDrawCommand* Borrow(uint32 threadId = 0);
    void Return(revDrawCommand* command, uint32 threadId = 0);

private:
    revArray<uint32> emptyBlock;
    revDrawCommand** storage;
};

#endif /* defined(__REVFRAWCOMMAND_H__) */
