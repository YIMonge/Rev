#ifndef __REVMATERIAL_H__
#define __REVMATERIAL_H__

#include "revShader.h"
#include "revTexture.h"
#include "revArray.h"
#include "revSerializer.h"

class revMaterial
{
public:
    revMaterial(){}
    virtual ~revMaterial(){}

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

        void SetBlendOpColor(BLEND_OP op){ blendOpColor = static_cast<uint8>(op); }
        void SetBlendFactorSrcColor(BLEND_FACTOR factor){ blendFactorSrcColor = static_cast<uint8>(factor); }
        void SetBlendFactorDstColor(BLEND_FACTOR factor){ blendFactorDstColor = static_cast<uint8>(factor); }
        void SetBlendOpAlpha(BLEND_OP op) { blendOpAlpha = static_cast<uint8>(op); }
        void SetBlendFactorSrcAlpha(BLEND_FACTOR factor){ blendFactorSrcAlpha = static_cast<uint8>(factor); }
        void SetBlendFactorDstAlpha(BLEND_FACTOR factor){ blendFactorDstAlpha = static_cast<uint8>(factor); }
        void SetEnableBlend(bool flag) { enable = flag; }


        SERIALIZE_FUNC()
        {
            archive(REV_NVP(colorMask),
                REV_NVP(blendOpColor),
                REV_NVP(blendFactorSrcColor),
                REV_NVP(blendFactorDstColor),
                REV_NVP(blendOpAlpha),
                REV_NVP(blendFactorSrcAlpha),
                REV_NVP(blendFactorDstAlpha),
                REV_NVP(enable)
            );
        }

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
        cullModeFlag(static_cast<uint8>(CULL_MODE_FLAG::NONE)),
        polygonMode(static_cast<uint8>(POLYGON_MODE::FILL))
        {}

        CULL_MODE_FLAG GetCullMode() const { return static_cast<CULL_MODE_FLAG>(cullModeFlag); }
        POLYGON_MODE GetPolygonMode() const { return static_cast<POLYGON_MODE>(polygonMode); }

        void SetCullMode(CULL_MODE_FLAG flag) { cullModeFlag = static_cast<uint8>(flag); }
        void GetPolygonMode(POLYGON_MODE mode) { polygonMode = static_cast<uint8>(mode); }

        SERIALIZE_FUNC()
        {
            archive(REV_NVP(cullModeFlag),
                REV_NVP(polygonMode)
            );
        }
    
    private:
        uint8 cullModeFlag;
        uint8 polygonMode;
    };

    const revShader* GetShader(SHADER_TYPE type) const { return shader[static_cast<uint32>(type)]; }
    const revArray<revTexture*> GetTextures() const { return textures; }
    const BlendState& GetBlendState() const { return blend; }
    const RasterizationState& GetRasterization() const { return rasterization; }
    bool isDirty() const { return dirty; }


    void SetShader(SHADER_TYPE type, revShader* shader)
    {
        this->shader[static_cast<uint32>(type)] = shader;
        dirty = true;
    }

    void SetTexture(const revArray<revTexture*> textures)
    {
        this->textures = textures;
        dirty = true;
    }

    void SetBlendState(const BlendState& blendState)
    {
        blend = blendState;
        dirty = true;
    }

    void SetRasterizationState(const RasterizationState& rasterizationState)
    {
        rasterization = rasterizationState;
        dirty = true;
    }

    // for serialize 
    // TODO: auto generation 
    SERIALIZE_FUNC()
    {
        archive(REV_NVP(blend),
            REV_NVP(rasterization)
        );
    }


protected:
    revShader* shader[2];
    revArray<revTexture*> textures;
    BlendState blend;
    RasterizationState rasterization;
    bool dirty;
};

#endif
