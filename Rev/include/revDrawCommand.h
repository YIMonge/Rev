//
//  DrawCommand.h
//  Rev
//
//  Created by yoike on 2013/10/17.
//  Copyright (c) 2013年 yoike. All rights reserved.
//

#ifndef __REVFRAWCOMMAND_H__
#define __REVFRAWCOMMAND_H__

#include "revMatrix44.h"
#include "revColor.h"
#include "revTypedef.h"


class revMaterial;

// Draw command type
enum DRAW_COMMAND_TYPE
{
    DCT_BEGINE = 0,
    DCT_END,
    DCT_CLEAR_COLOR,
    DCT_CLEAR_DEPTH,
    DCT_DRAW,
    DCT_MAX_NUM,
};

struct revDrawCommand
{
    DRAW_COMMAND_TYPE   type;               // 
    uint32              program;            // 
    uint32              vertex_buffer;      // 
    revMaterial*        material;           // 
    revMatrix44*        world;              // 
    revMatrix44*        locals;             // 
};

#endif /* defined(__REVFRAWCOMMAND_H__) */
