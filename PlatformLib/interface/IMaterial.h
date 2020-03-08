#ifndef __IMATERIAL_H__
#define __IMATERIAL_H__

#include "IShader.h"
#include "ITexture.h"
#include "revArray.h"

class IMaterial
{
public:
    IMaterial(){}
    virtual ~IMaterial(){}

    const IShader* GetShader(SHADER_TYPE type) const { return shader[static_cast<uint32>(type)]; }
    const revArray<ITexture*> GetTextures() const { return textures; }
protected:
    IShader* shader[2];
    revArray<ITexture*> textures;
};

#endif
