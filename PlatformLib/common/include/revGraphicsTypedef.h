#ifndef __GRAPHICS_TYPEDEF_H__
#define __GRAPHICS_TYPEDEF_H__

#include "revTypedef.h"
#include "revMath.h"

// TODO: if values are serialized should have origin value like below
// enum class FILTER {
//    A = 0,
//    B = 1,
// };
// uint32 ConvertToVulkanFilter(FILTER filter) { return table[(uint32)filter]; }


struct GraphicsDesc
{
    enum BUFFERTYPE
    {
        DOUBLE_BUFFER = 0,
        TRIPLE_BUFFER,
        BUFFERTYPE_MAX_NUM,
    };

    BUFFERTYPE		bufferType;
    bool			useFullscreen;

    uint32 GetBufferNum() const
    {
        return bufferType == BUFFERTYPE::DOUBLE_BUFFER ? 2 : 3;
    }
};

enum class FILTER_MODE {
    NEAREST,
    LINEAR,
    MAX_NUM,
};

enum class MIP_FILTER_MODE {
	NEAREST,
	LINEAR,
	NONE,
	MAX_NUM,
};

enum class TEXTURE_ADDRESS_MODE
{
	WRAP,
	MIRROR,
	CLAMP,
	BORDER,
	MIRROR_ONCE,
	MAX_NUM,
};

enum class COMPARISON_FUNC
{
	NEVER,
	LESS,
	EQUAL,
	LESS_EQUAL,
	GREATER,
	NOT_EQUAL,
	GREATER_EQUAL,
	ALWAYS,
	MAX_NUM,
};

enum class BORDER_COLOR_MODE
{
	TRANSPARENT_BLACK,
	OPAQUE_BLACK,
	OPAQUE_WHITE,
};

enum class GRAPHICS_FORMAT
{
	R8G8B8A8_UNORM,
	R32G32_SFLOAT,
	R32G32B32_SFLOAT,
};

// belows are specific graphics lib arguments.
#ifdef _USE_VULKAN
#include "../lib/vulkan_wrapper.h"
using revGraphicsDevice = VkDevice;
using revSwapChain = VkSwapchainKHR;
using revShaderHandle = VkShaderModule;
using revGraphicsResource = VkBuffer;
using revGraphicsCommandBuffer = VkCommandBuffer;
using revGraphicsCommandQueue = VkQueue;
using revTextureHandle = VkImage;
using revTextureResourceView = VkImageView;
using revTextureSampler = VkSampler;

enum class COLOR_COMPONENT_FRAG
{
    R = VK_COLOR_COMPONENT_R_BIT,
    G = VK_COLOR_COMPONENT_G_BIT,
    B = VK_COLOR_COMPONENT_B_BIT,
    A = VK_COLOR_COMPONENT_A_BIT,
    ALL = R | G| B | A
};

enum class LOGIC_OP
{
    CLEAR                       = VK_LOGIC_OP_CLEAR,
    AND                         = VK_LOGIC_OP_AND,
    AND_REVERSE                 = VK_LOGIC_OP_AND_REVERSE,
    COPY                        = VK_LOGIC_OP_COPY,
    AND_INVERTED                = VK_LOGIC_OP_AND_INVERTED,
    NO_OP                       = VK_LOGIC_OP_NO_OP,
    XOR                         = VK_LOGIC_OP_XOR,
    OR                          = VK_LOGIC_OP_OR,
    NOR                         = VK_LOGIC_OP_NOR,
    EQUIVALENT                  = VK_LOGIC_OP_EQUIVALENT,
    INVERT                      = VK_LOGIC_OP_INVERT,
    OR_REVERSE                  = VK_LOGIC_OP_OR_REVERSE,
    //VK_LOGIC_OP_COPY_INVERTED,
    //VK_LOGIC_OP_OR_INVERTED,
    //VK_LOGIC_OP_NAND,
    //VK_LOGIC_OP_SET,
};

enum class BLEND_OP
{
	ADD					= VK_BLEND_OP_ADD,
	SUBTRACT			= VK_BLEND_OP_SUBTRACT,
	REVERSE_SUBTRACT	= VK_BLEND_OP_REVERSE_SUBTRACT,
	MIN					= VK_BLEND_OP_MIN,
};


enum class BLEND_FACTOR
{
    ZERO = VK_BLEND_FACTOR_ZERO,
    ONE = VK_BLEND_FACTOR_ONE,
    SRC_COLOR = VK_BLEND_FACTOR_SRC_COLOR,
    SRC_ALPHA = VK_BLEND_FACTOR_SRC_ALPHA,
    DST_COLOR = VK_BLEND_FACTOR_DST_COLOR,
    DST_ALPHA = VK_BLEND_FACTOR_DST_ALPHA,
    ONE_MINUS_SRC_COLOR = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
    ONE_MINUS_SRC_ALPHA = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    ONE_MINUS_DST_COLOR= VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
    ONE_MINUS_DST_ALPHA= VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
};

enum class CULL_MODE_FLAG
{
    NONE = VK_CULL_MODE_NONE,
    FRONT = VK_CULL_MODE_FRONT_BIT,
    BACK = VK_CULL_MODE_BACK_BIT,
    FRONT_AND_BACK = VK_CULL_MODE_FRONT_AND_BACK,
};

enum class POLYGON_MODE
{
    FILL = VK_POLYGON_MODE_FILL,
    LINE = VK_POLYGON_MODE_LINE,
};

namespace {
    VkFilter ConvertToVKFilter(FILTER_MODE filter)
    {
        const VkFilter table[] = {
            VK_FILTER_NEAREST,
            VK_FILTER_LINEAR,
        };
        return table[static_cast<uint32>(filter)];
    }

    VkSamplerMipmapMode ConvertToVKMipFilterMode(MIP_FILTER_MODE mode)
	{
    	const VkSamplerMipmapMode table[] = {
			VK_SAMPLER_MIPMAP_MODE_NEAREST,
			VK_SAMPLER_MIPMAP_MODE_LINEAR,
			VK_SAMPLER_MIPMAP_MODE_LINEAR,	// dummy
    	};
    	return table[static_cast<uint32>(mode)];
	}

	VkSamplerAddressMode ConvertToVKTextureAddressMode(TEXTURE_ADDRESS_MODE mode)
	{
    	const VkSamplerAddressMode table[] = {
			VK_SAMPLER_ADDRESS_MODE_REPEAT,
			VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
			VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
			VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
			VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE,
		};
    	return table[static_cast<uint32>(mode)];
	}

	VkCompareOp ConverToVKComparisonFunc(COMPARISON_FUNC comparisonFunc)
	{
    	const VkCompareOp table[] = {
			VK_COMPARE_OP_NEVER,
			VK_COMPARE_OP_LESS,
			VK_COMPARE_OP_EQUAL,
			VK_COMPARE_OP_LESS_OR_EQUAL,
			VK_COMPARE_OP_GREATER,
			VK_COMPARE_OP_NOT_EQUAL,
			VK_COMPARE_OP_GREATER_OR_EQUAL,
			VK_COMPARE_OP_ALWAYS,
    	};
    	return table[static_cast<uint32>(comparisonFunc)];
	}

	VkBorderColor ConvertToVKBorderColor(BORDER_COLOR_MODE mode)
	{
    	const VkBorderColor table[] = {
			VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK,
			VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,
			VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
    	};
    	return table[static_cast<uint32>(mode)];
	}

	VkFormat ConvertToVKFormat(GRAPHICS_FORMAT format)
	{
		const VkFormat table[] = {
			VK_FORMAT_R8G8B8A8_UNORM,
			VK_FORMAT_R32G32_SFLOAT,
			VK_FORMAT_R32G32B32_SFLOAT,
		};
		return table[static_cast<uint32>(format)];
	}
}

#elif defined(_USE_DIRECTX12)
#include <d3d12.h>
#include <dxgi1_4.h>
#include "libs/DX12/d3dx12.h"

using revGraphicsDevice = ID3D12Device*;
using revSwapChain = IDXGISwapChain3*;
using revShaderHandle = ID3DBlob*;
using revGraphicsResource = ID3D12Resource*;
using revGraphicsCommandBuffer = ID3D12GraphicsCommandList*;
using revGraphicsCommandQueue = ID3D12CommandQueue*;
using revTextureHandle = ID3D12Resource;
using revTextureResourceView = D3D12_CPU_DESCRIPTOR_HANDLE;
using revTextureSampler = D3D12_GPU_DESCRIPTOR_HANDLE;
using revGraphicsHeap = ID3D12DescriptorHeap;



// TODO: DX12 
enum class COLOR_COMPONENT_FRAG
{
    R = D3D12_COLOR_WRITE_ENABLE_RED,
    G = D3D12_COLOR_WRITE_ENABLE_GREEN,
    B = D3D12_COLOR_WRITE_ENABLE_BLUE,
    A = D3D12_COLOR_WRITE_ENABLE_ALPHA,
    ALL = D3D12_COLOR_WRITE_ENABLE_ALL,
};

enum class LOGIC_OP
{
    CLEAR           = D3D12_LOGIC_OP_CLEAR,
    AND             = D3D12_LOGIC_OP_AND,
    AND_REVERSE     = D3D12_LOGIC_OP_AND_REVERSE,
    COPY            = D3D12_LOGIC_OP_COPY,
    AND_INVERTED    = D3D12_LOGIC_OP_AND_INVERTED,
    NO_OP           = D3D12_LOGIC_OP_NOOP,
    XOR             = D3D12_LOGIC_OP_XOR,
    OR              = D3D12_LOGIC_OP_OR,
    NOR             = D3D12_LOGIC_OP_NOR,
    EQUIVALENT      = D3D12_LOGIC_OP_EQUIV,
    INVERT          = D3D12_LOGIC_OP_INVERT,
    OR_REVERSE      = D3D12_LOGIC_OP_OR_REVERSE,
};

enum class BLEND_OP
{
	ADD					= D3D12_BLEND_OP_ADD,
	SUBTRACT			= D3D12_BLEND_OP_SUBTRACT,
	REVERSE_SUBTRACT	= D3D12_BLEND_OP_REV_SUBTRACT,
	MIN					= D3D12_BLEND_OP_MIN,
};

enum class BLEND_FACTOR
{
    ZERO                = D3D12_BLEND_ZERO,
    ONE                 = D3D12_BLEND_ONE,
    SRC_COLOR           = D3D12_BLEND_SRC_COLOR,
    SRC_ALPHA           = D3D12_BLEND_SRC_ALPHA,
    DST_COLOR           = D3D12_BLEND_DEST_COLOR,
    DST_ALPHA           = D3D12_BLEND_DEST_ALPHA,
    ONE_MINUS_SRC_COLOR = D3D12_BLEND_INV_SRC_COLOR,
    ONE_MINUS_SRC_ALPHA = D3D12_BLEND_INV_SRC_ALPHA,
    ONE_MINUS_DST_COLOR = D3D12_BLEND_INV_DEST_COLOR,
    ONE_MINUS_DST_ALPHA = D3D12_BLEND_INV_DEST_ALPHA,
};

enum class CULL_MODE_FLAG
{
    NONE    = D3D12_CULL_MODE_NONE,
    FRONT   = D3D12_CULL_MODE_FRONT,
    BACK    = D3D12_CULL_MODE_BACK,
    FRONT_AND_BACK,
};

enum class POLYGON_MODE
{
    FILL = D3D12_FILL_MODE_SOLID,
    LINE = D3D12_FILL_MODE_WIREFRAME,
};


namespace {
	REV_INLINE D3D12_FILTER ConvertToDXFilter(FILTER_MODE minFilter, FILTER_MODE magFilter, MIP_FILTER_MODE mip, uint32 anisotorpy, bool compare)
	{
		D3D12_FILTER_REDUCTION_TYPE reduction = D3D12_FILTER_REDUCTION_TYPE_STANDARD;
		if (compare) reduction = D3D12_FILTER_REDUCTION_TYPE_COMPARISON;
		if (anisotorpy != 0) return D3D12_ENCODE_ANISOTROPIC_FILTER(reduction);
		return D3D12_ENCODE_BASIC_FILTER(static_cast<uint32>(minFilter),
			static_cast<uint32>(magFilter),
			static_cast<uint32>(mip),
			reduction);
	}

	REV_INLINE D3D12_TEXTURE_ADDRESS_MODE ConverToDXTextureAddressMode(TEXTURE_ADDRESS_MODE mode)
	{
		const D3D12_TEXTURE_ADDRESS_MODE table[] = {
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_BORDER,
			D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE,
		};
		return table[static_cast<uint32>(mode)];
	}

	REV_INLINE D3D12_COMPARISON_FUNC ConvertToDXComparisonFunc(COMPARISON_FUNC comparison)
	{
		const D3D12_COMPARISON_FUNC table[] = {
			D3D12_COMPARISON_FUNC_NEVER,
			D3D12_COMPARISON_FUNC_LESS,
			D3D12_COMPARISON_FUNC_EQUAL,
			D3D12_COMPARISON_FUNC_LESS_EQUAL,
			D3D12_COMPARISON_FUNC_GREATER,
			D3D12_COMPARISON_FUNC_NOT_EQUAL,
			D3D12_COMPARISON_FUNC_GREATER_EQUAL,
			D3D12_COMPARISON_FUNC_ALWAYS
		};
		return table[static_cast<uint32>(comparison)];
	}

	REV_INLINE D3D12_STATIC_BORDER_COLOR ConvertToDXBorderColor(BORDER_COLOR_MODE mode)
	{
		const D3D12_STATIC_BORDER_COLOR table[] = {
			D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK,
			D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK,
			D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE
		};
		return table[static_cast<uint32>(mode)];
	}


	REV_INLINE DXGI_FORMAT ConvertToDXFormat(GRAPHICS_FORMAT format)
	{
		const DXGI_FORMAT table[] = {
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_R32G32B32_FLOAT,
		};
		return table[static_cast<uint32>(format)];
	}
}



/*
sampler_desc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
sampler_desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
sampler_desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
sampler_desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
sampler_desc.MipLODBias = 0.0f;
sampler_desc.MaxAnisotropy = 16;
sampler_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
sampler_desc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
sampler_desc.MinLOD = 0.0f;
sampler_desc.MaxLOD = D3D12_FLOAT32_MAX;
sampler_desc.ShaderRegister = 0;
sampler_desc.RegisterSpace = 0;
sampler_desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
*/

#endif


#endif
