#ifndef __REVTEXTURE_H__
#define __REVTEXTURE_H__

#include "revGraphicsTypedef.h"
#include "revResource.h"
#include "revDevice.h"
#include "Log.h"

class revTexture : public revResource
{
public:
	revTexture() {}
	virtual ~revTexture() {}

	const revTextureSampler& GetSampler() const { return sampler; }
	GRAPHICS_FORMAT GetFormat() const { return static_cast<GRAPHICS_FORMAT>(metaData.format); }
	uint32 GetWidth() const { return width; }
	uint32 GetHeight() const { return height; }
	revTextureHandle* GetHandle() const { return handle; }


	bool LoadFromFile(revDevice* device, const char* path);

    class SamplerDesc {
    public:
        SamplerDesc() {
			SetMagFilter(FILTER_MODE::LINEAR);
			SetMinFilter(FILTER_MODE::LINEAR);
			SetMipFilter(MIP_FILTER_MODE::LINEAR);
			SetAddressModeU(TEXTURE_ADDRESS_MODE::WRAP);
			SetAddressModeV(TEXTURE_ADDRESS_MODE::WRAP);
			SetAddressModeW(TEXTURE_ADDRESS_MODE::WRAP);
			SetMipLodBias(0.0f);
			SetMaxAnisotropy(1);
			SetComparisonFunc(COMPARISON_FUNC::NEVER);
			SetMinLod(0.0f);
			SetMaxLod(1.0f);
			SetBorderColor(BORDER_COLOR_MODE::OPAQUE_BLACK);
        }

		FILTER_MODE GetMagFilter() const { return static_cast<FILTER_MODE>(magFilter); }
		FILTER_MODE GetMinFilter() const { return static_cast<FILTER_MODE>(minFilter); }
		MIP_FILTER_MODE GetMipFilter() const { return static_cast<MIP_FILTER_MODE>(mipFilter); }
		TEXTURE_ADDRESS_MODE GetAddressModeU() const { return static_cast<TEXTURE_ADDRESS_MODE>(addressU); }
		TEXTURE_ADDRESS_MODE GetAddressModeV() const { return static_cast<TEXTURE_ADDRESS_MODE>(addressV); }
		TEXTURE_ADDRESS_MODE GetAddressModeW() const { return static_cast<TEXTURE_ADDRESS_MODE>(addressW); }
		f32 GetMipLodBias() const { return mipLodBias; }
		uint32 GetMaxAnisotropy() const { return maxAnisotropy; }
		COMPARISON_FUNC GetComparisonFunc() const { return static_cast<COMPARISON_FUNC>(comparisonFunc); }
		f32 GetMinLod() const { return minLod; }
		f32 GetMaxLod() const { return maxLod; }
		BORDER_COLOR_MODE GetBorderColor() const { return static_cast<BORDER_COLOR_MODE>(borderColor); }

		void SetMagFilter(FILTER_MODE mode) { magFilter = static_cast<uint8>(mode); }
		void SetMinFilter(FILTER_MODE mode){ minFilter = static_cast<uint8>(mode); }
		void SetMipFilter(MIP_FILTER_MODE mode) { mipFilter = static_cast<uint8>(mode); }
		void SetAddressModeU(TEXTURE_ADDRESS_MODE mode) { addressU = static_cast<uint8>(mode); }
		void SetAddressModeV(TEXTURE_ADDRESS_MODE mode) { addressV = static_cast<uint8>(mode); }
		void SetAddressModeW(TEXTURE_ADDRESS_MODE mode) { addressW = static_cast<uint8>(mode); }
		void SetMipLodBias(f32 mipBias) { mipLodBias = mipBias; }
		void SetMaxAnisotropy(uint32 anisotropy){ maxAnisotropy = anisotropy; }
		void SetComparisonFunc(COMPARISON_FUNC comparison) { comparisonFunc = static_cast<uint8>(comparison); }
		void SetMinLod(f32 value) { minLod = value; }
		void SetMaxLod(f32 value) { maxLod = value; }
		void SetBorderColor(BORDER_COLOR_MODE mode) { borderColor = static_cast<uint8>(mode); }



        SERIALIZE_FUNC() {
            archive(REV_NVP(magFilter),
                REV_NVP(minFilter),
				REV_NVP(mipFilter),
                REV_NVP(addressU),
                REV_NVP(addressV),
                REV_NVP(addressW),
                REV_NVP(mipLodBias),
                REV_NVP(maxAnisotropy),
                REV_NVP(comparisonFunc),
                REV_NVP(minLod),
                REV_NVP(maxLod),
                REV_NVP(borderColor)
                );
        }
    private:
        uint8 magFilter;
        uint8 minFilter;
		uint8 mipFilter;
        uint8 addressU;
        uint8 addressV;
        uint8 addressW;
        f32   mipLodBias;
        uint8 maxAnisotropy;
        uint8 comparisonFunc;
        f32   minLod;
        f32	  maxLod;
        uint8 borderColor;
    };

    struct TextureMetaData
    {
		DefaultMetaData data;
		uint32 format;
		SamplerDesc sampler;
    };
	
	
protected:
	virtual bool CreateTexture(revDevice* device, uint8* imageData) = 0;

	uint32 width;
	uint32 height;
	revTextureHandle* handle;
	TextureMetaData metaData;
    revTextureSampler sampler;
};
 

SERIALIZE_FUNC_NON_INTRUSIVE(revTexture::TextureMetaData, metaData)
{
	archive(REV_MAKE_NVP(data, metaData.data),
		REV_MAKE_NVP(format, metaData.format),
		REV_MAKE_NVP(sampler, metaData.sampler));
}



#endif