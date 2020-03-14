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

    // TODO: serialize to binary file and deserialize from binary file.
    class BlendState
    {
    public:
        BlendState() :
        colorMask(static_cast<uint8>(COLOR_COMPONENT_FRAG::ALL)),
        enable(false)
        {}

        COLOR_COMPONENT_FRAG  GetWriteColorMask() const
        {
            return static_cast<COLOR_COMPONENT_FRAG>(colorMask);
        }

        BLEND_OP GetBlendOpColor() const { return static_cast<BLEND_OP >(blendOpColor); }
        BLEND_FACTOR GetBlendFactorSrcColor() const { return static_cast<BLEND_FACTOR>(blendFactorSrcColor); }
        BLEND_FACTOR GetBlendFactorDstColor() const { return static_cast<BLEND_FACTOR>(blendFactorDstColor); }
        BLEND_OP GetBlendOpAlpha() const { return static_cast<BLEND_OP >(blendOpAlpha); }
        BLEND_FACTOR GetBlendFactorSrcAlpha() const { return static_cast<BLEND_FACTOR>(blendFactorSrcAlpha); }
        BLEND_FACTOR GetBlendFactorDstAlpha() const { return static_cast<BLEND_FACTOR>(blendFactorDstAlpha); }
        bool isEnableBlend() const { return enable; }
    private:
        uint8 colorMask;
        uint8 blendOpColor;
        uint8 blendFactorSrcColor;
        uint8 blendFactorDstColor;
        uint8 blendOpAlpha;
        uint8 blendFactorSrcAlpha;
        uint8 blendFactorDstAlpha;
        bool enable;
    };

    class DepthStencilState
    {
    public:
        DepthStencilState(){}

    private:

    };

    class RasterizationState
    {
    public:
        RasterizationState():
        cullModeFlag(CULL_MODE_FLAG::NONE),
        polygonMode(POLYGON_MODE::FILL)
        {}

        CULL_MODE_FLAG GetCullMode() const { return cullModeFlag; }
        POLYGON_MODE GetPolygonMode() const { return polygonMode; }
    private:
        CULL_MODE_FLAG cullModeFlag;
        POLYGON_MODE polygonMode;
    };

    const IShader* GetShader(SHADER_TYPE type) const { return shader[static_cast<uint32>(type)]; }
    const revArray<ITexture*> GetTextures() const { return textures; }
    const BlendState& GetBlendState() const { return blend; }
    const RasterizationState& GetRasterization() const { return rasterization; }
protected:
    IShader* shader[2];
    revArray<ITexture*> textures;
    BlendState blend;
    RasterizationState rasterization;
};

#endif
