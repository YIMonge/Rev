#ifndef __REVMATERIAL_H__
#define __REVMATERIAL_H__

#include "revResource.h"
#include "revShader.h"
#include "revTexture.h"

class revMaterial : public revResource
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
			enableLogicOp(false),
			logicOp(static_cast<uint8>(LOGIC_OP::AND)),
			enableBlend(false),
			enableAlphaToCoverage(false)
        {}

		uint8  GetWriteColorMask() const { return colorMask; }
        BLEND_OP GetBlendOpColor() const { return static_cast<BLEND_OP >(blendOpColor); }
        BLEND_FACTOR GetBlendFactorSrcColor() const { return static_cast<BLEND_FACTOR>(blendFactorSrcColor); }
        BLEND_FACTOR GetBlendFactorDstColor() const { return static_cast<BLEND_FACTOR>(blendFactorDstColor); }
        BLEND_OP GetBlendOpAlpha() const { return static_cast<BLEND_OP >(blendOpAlpha); }
        BLEND_FACTOR GetBlendFactorSrcAlpha() const { return static_cast<BLEND_FACTOR>(blendFactorSrcAlpha); }
        BLEND_FACTOR GetBlendFactorDstAlpha() const { return static_cast<BLEND_FACTOR>(blendFactorDstAlpha); }
		LOGIC_OP GetLogicOp() const { return static_cast<LOGIC_OP>(logicOp); }
		bool isEnabledLogicOp() const { return enableLogicOp; }
		bool isEnableBlend() const { return enableBlend; }
		bool isEbaleAlphaToCoverage() const{ return enableAlphaToCoverage; }

		void SetWriteColorMask(uint8 mask) { colorMask = mask; }
        void SetBlendOpColor(BLEND_OP op){ blendOpColor = static_cast<uint8>(op); }
        void SetBlendFactorSrcColor(BLEND_FACTOR factor){ blendFactorSrcColor = static_cast<uint8>(factor); }
        void SetBlendFactorDstColor(BLEND_FACTOR factor){ blendFactorDstColor = static_cast<uint8>(factor); }
        void SetBlendOpAlpha(BLEND_OP op) { blendOpAlpha = static_cast<uint8>(op); }
        void SetBlendFactorSrcAlpha(BLEND_FACTOR factor){ blendFactorSrcAlpha = static_cast<uint8>(factor); }
        void SetBlendFactorDstAlpha(BLEND_FACTOR factor){ blendFactorDstAlpha = static_cast<uint8>(factor); }
		void SetLogicOp(LOGIC_OP op) { logicOp = static_cast<uint8>(op); }
		void SetEnableLogicOp(bool flag) { enableLogicOp = flag; }
        void SetEnableBlend(bool flag) { enableBlend = flag; }
		void SetEnableAlphaToCoverage(bool flag) { enableAlphaToCoverage = flag; }


        SERIALIZE_FUNC()
        {
            archive(REV_NVP(colorMask),
                REV_NVP(blendOpColor),
                REV_NVP(blendFactorSrcColor),
                REV_NVP(blendFactorDstColor),
                REV_NVP(blendOpAlpha),
                REV_NVP(blendFactorSrcAlpha),
                REV_NVP(blendFactorDstAlpha),
				REV_NVP(enableLogicOp),
				REV_NVP(logicOp),
                REV_NVP(enableBlend),
				REV_NVP(enableAlphaToCoverage)
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
		bool  enableLogicOp;
		uint8 logicOp;
        bool  enableBlend;
		bool  enableAlphaToCoverage;
    };

    class DepthStencilState
    {
    public:
        DepthStencilState() : 
			enableDepthTest(true),
			enableDepthWrite(true),
			enableStanecil(false),
			depthFunc(static_cast<uint8>(COMPARISON_FUNC::LESS))
		{}

		bool IsEnableDepthTest() const { return enableDepthTest; }
		bool IsEnableDepthWrite() const { return enableDepthWrite; }
		COMPARISON_FUNC GetDepthFunc() const { return static_cast<COMPARISON_FUNC>(depthFunc); }

		bool IsEnableStencil() const { return enableStanecil; }

		SERIALIZE_FUNC()
		{
			archive(REV_NVP(enableDepthTest),
				REV_NVP(enableDepthWrite),
				REV_NVP(enableStanecil),
				REV_NVP(depthFunc)
			);
		}
    private:
		uint8 depthFunc;
		bool enableDepthTest;
		bool enableDepthWrite;
		bool enableStanecil;

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
        void SetPolygonMode(POLYGON_MODE mode) { polygonMode = static_cast<uint8>(mode); }

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

	// Material parameter that includes color or texture.
	class Property
	{
	public:
		Property() :
			type(TYPE::NOT_INITIALIZED)
		{}
		Property(revColor color, const revString& name) :
			color(color),
			name(name),
			type(TYPE::COLOR)
		{}
		Property(revTexture* texture, const revString& name) :
			texture(texture),
			name(name),
			type(TYPE::TEXTURE)
		{
			uuid = texture->GetUUID();
		}
		Property(f32 value, const revString& name) :
			value(value),
			name(name),
			type(TYPE::FLOAT)
		{
		}


		enum class TYPE
		{
			NOT_INITIALIZED,
			TEXTURE,
			COLOR,
			FLOAT,
		};


		TYPE GetType() const { return type; }
		f32 GetValue() const {
			if (type != TYPE::FLOAT) NATIVE_LOGE("Material Property Type miss match. file:%s, line:%s", __FILE__, __LINE__);
			return value;
		}
		revTexture* GetTexture() const {
			if (type != TYPE::TEXTURE) NATIVE_LOGE("Material Property Type miss match. file:%s, line:%s", __FILE__, __LINE__);
			return texture;
		}
		const revColor& GetColor() const {
			if (type != TYPE::COLOR) NATIVE_LOGE("Material Property Type miss match. file:%s, line:%s", __FILE__, __LINE__);
			return color;
		}
		const revString& GetName() const { return name; }

		void SetValue(f32 value) {
			if (type != TYPE::FLOAT) {
				if (type == TYPE::TEXTURE) texture = nullptr;
			}
			this->value = value;
			type = TYPE::FLOAT;
		}
		void SetTexture(revTexture* texture){
			this->texture = texture;
			type = TYPE::TEXTURE;
		}
		void SetColor(const revColor& color)
		{
			this->color = color;
			type = TYPE::COLOR;
		}
		void SetName(const revString& name)
		{
			this->name = name;
		}
		void SetName(const char* name)
		{
			this->name = name;
		}

	private:
		union {
			struct {
				revTexture* texture;
				uint64 uuid;
			};
			struct {
				f32 value;
				uint32 padding[3];
			};
			revColor color;
		};
		revString name;
		TYPE type;
	};


    const revShader* GetShader(SHADER_TYPE type) const { return shader[static_cast<uint32>(type)]; }
    const revArray<Property>& GetProperties() const { return properties; }
	const Property& GetProperty(uint32 index) const {
		if(index >= static_cast<uint32>(properties.size())) NATIVE_LOGE("Out of range Material property. file:%s, line:%s", __FILE__, __LINE__);
		return properties[index];
	}
	const Property& GetProperty(const revString& propertyName)
	{
		for (auto property : properties)
		{
			if (property.GetName() == propertyName) {
				return property;
			}
		}
		NATIVE_LOGI("Could not found property:%s, file:%s, line:%s", propertyName, __FILE__, __LINE__);
		return properties[0];
	}
    const BlendState& GetBlendState() const { return blend; }
	const DepthStencilState& GetDepthStencil() const { return depthStencil; }
    const RasterizationState& GetRasterization() const { return rasterization; }
    bool isDirty() const { return dirty; }


    void SetShader(SHADER_TYPE type, revShader* shader)
    {
        this->shader[static_cast<uint32>(type)] = shader;
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
	void AddProperty(const Property& property)
	{
		properties.push_back(property);
		dirty = true;
	}

	const revShader* const GetVertexShader() const { return shader[0]; }
	const revShader* const GetPixelShader() const { return shader[1]; }

    SERIALIZE_FUNC()
    {
        archive(
			SERIALIZE_BASE_CLASS(revResource),
			REV_NVP(blend),
			REV_NVP(depthStencil),
            REV_NVP(rasterization)
        );
    }


protected:
    revShader* shader[2];
    revArray<Property> properties; 
    BlendState blend;
	DepthStencilState depthStencil;
    RasterizationState rasterization;


    bool dirty;
};

#endif
