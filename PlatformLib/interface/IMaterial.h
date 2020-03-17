#ifndef __IMATERIAL_H__
#define __IMATERIAL_H__

#include "IShader.h"

class IMaterial
{

protected:
    IShader* vertexShader;
    IShader* fragmentShader;
};

#endif
