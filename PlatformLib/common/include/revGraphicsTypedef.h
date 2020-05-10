#ifndef __REVGRAPHICSTYPEDEF_H__
#define __REVGRAPHICSTYPEDEF_H__

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
	R32_UINT,
	R32_SINT,
	R32_FLOAT,
	R32G32_UINT,
	R32G32_SINT,
	R32G32_FLOAT,
	R32G32B32_UINT,
	R32G32B32_SINT,
	R32G32B32_FLOAT,
	R32G32B32A32_UINT,
	R32G32B32A32_SINT,
	R32G32B32A32_FLOAT,
	MAX_NUM,
};

enum class BLEND_FACTOR
{
	ZERO,
	ONE,
	SRC_COLOR,
	SRC_ALPHA,
	DST_COLOR,
	DST_ALPHA,
	ONE_MINUS_SRC_COLOR,
	ONE_MINUS_SRC_ALPHA,
	ONE_MINUS_DST_COLOR,
	ONE_MINUS_DST_ALPHA,
	MAX_NUM,
};

enum class BLEND_OP
{
	ADD,
	SUBTRACT,
	REVERSE_SUBTRACT,
	MIN,
	MAX_NUM,
};

enum class LOGIC_OP
{
	CLEAR,
	AND,
	AND_REVERSE,
	COPY,
	AND_INVERTED,
	NO_OP,
	XOR,
	OR,
	NOR,
	EQUIVALENT,
	INVERT,
	OR_REVERSE,
	MAX_NUM,
};

enum class POLYGON_MODE
{
	FILL,
	LINE,
	MAX_NUM,
};


enum class CULL_MODE_FLAG
{
	NONE,
	FRONT,
	BACK,
	FRONT_AND_BACK,
	MAX_NUM,
};

enum class COLOR_COMPONENT_FRAG : uint8
{
	R = 1,
	G = 2,
	B = 4,
	A = 8,
	ALL = R | G | B | A
};

enum class INPUT_ELEMENT_TYPE
{
	POSITION,
	NORMAL,
	TANGENT,
	TEXCOORD0,
	TEXCOORD1,
	TEXCOORD2,
	TEXCOORD3,
	COLOR0,
	COLOR1,
	COLOR2,
	COLOR3,
	MAX_NUM,
};

struct SemanticData
{
	const char* name;
	uint32 index;
	GRAPHICS_FORMAT format;
	uint32 sizeOfBytes;
};

// TODO: split to cpp
const SemanticData GRAPHICS_SEMANTICS[] =
{
		{"POSITION",	0,  GRAPHICS_FORMAT::R32G32B32_FLOAT,		sizeof(revVector3), },
		{"NORMAL",		0,  GRAPHICS_FORMAT::R32G32B32_FLOAT,		sizeof(revVector3), },
		{"TANGENT",		0,	GRAPHICS_FORMAT::R32G32B32_FLOAT,		sizeof(revVector3), },
		{"TEXCOORD",	0,	GRAPHICS_FORMAT::R32G32_FLOAT,			sizeof(revVector2), },
		{"TEXCOORD",	1,  GRAPHICS_FORMAT::R32G32_FLOAT,			sizeof(revVector2), },
		{"TEXCOORD",	2,  GRAPHICS_FORMAT::R32G32_FLOAT,			sizeof(revVector2), },
		{"TEXCOORD",	3,  GRAPHICS_FORMAT::R32G32_FLOAT,			sizeof(revVector2), },
		{"COLOR",		0,  GRAPHICS_FORMAT::R32G32B32A32_FLOAT,	sizeof(revVector4), },
		{"COLOR",		1,  GRAPHICS_FORMAT::R32G32B32A32_FLOAT,	sizeof(revVector4), },
		{"COLOR",		2,  GRAPHICS_FORMAT::R32G32B32A32_FLOAT,	sizeof(revVector4), },
		{"COLOR",		3,  GRAPHICS_FORMAT::R32G32B32A32_FLOAT,	sizeof(revVector4), },
		{"MAX_NUM",		0,  GRAPHICS_FORMAT::MAX_NUM,				0, },
};

enum class INPUT_CLASS
{
	PER_VERTEX,
	PER_INSTANCE,
	MAX_NUM,
};

enum class DESCRIPTOR_TYPE
{
	TEXTURE_SHADER_RESOURCE_VIEW,
	RAW_BUFFER_SHADER_RESOURCE_VIEW,
	TYPED_BUFFER_SHADER_RESOURCE_VIEW,
	STRUCTURED_BUFFER_SHADER_RESOURCE_VIEW,
	
	TEXTURE_UNORDERED_ACCESS_VIEW,
	RAW_BUFFER_UNORDERED_RESOURCE_VIEW,
	TYPED_BUFFER_UNORDERED_RESOURCE_VIEW,
	STRUCTURED_BUFFER_UNORDERED_RESOURCE_VIEW,
	
	CONSTANT_BUFFER_VIEW,
	SAMPLER,
	MAX_NUM,
};

enum class SHADER_VISIBILITY
{
	VERTEX,
	PIXEL,
	GEOMETRY,
	HULL,
	ALL,
	MAX_NUM,
};

enum class DESCRIPTOR_HEAP_TYPE
{
	RESOURCE,
	SAMPLER,
	RENDER_TARGET,
	DEPTH_STENCIL,
	MAX_NUM,
};

enum class SHADER_INPUT_TYPE
{
	CBUFFER,
	TBUFFER,
	TEXTURE,
	SAMPLER,
	UAV_RWTYPED,
	STRUCTURED,
	UAV_RWSTRUCTURED,
	BYTEADDRESS,
	UAV_RWBYTEADDRESS,
	UAV_APPEND_STRUCTURED,
	UAV_CONSUME_STRUCTURED,
	UAV_RWSTRUCTURED_WITH_COUNTER,
	MAX_NUM,
};


// belows are specific graphics lib arguments.
#ifdef _USE_VULKAN
#include "../lib/vulkan_wrapper.h"
using revGraphicsDevice = VkDevice;
using revGraphicsAdapter = VkPhysicalDevice;
using revSwapChain = VkSwapchainKHR;
using revShaderHandle = VkShaderModule;
using revGraphicsResource = VkBuffer;
using revGraphicsCommandAllocator = VkCommandPool;
using revGraphicsCommandBuffer = VkCommandBuffer;
using revGraphicsCommandQueue = VkQueue;
using revGraphicsPipeline = VkPipeline;
using revTextureHandle = VkImage;
using revTextureSampler = VkSampler;
using revDescriptorHeap = void;

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
			VK_FORMAT_R32_UINT,
			VK_FORMAT_R32_SINT,
			VK_FORMAT_R32_SFLOAT,
			VK_FORMAT_R32G32_UINT,
			VK_FORMAT_R32G32_SINT,
			VK_FORMAT_R32G32_SFLOAT,
			VK_FORMAT_R32G32B32_UINT,
			VK_FORMAT_R32G32B32_SINT,
			VK_FORMAT_R32G32B32_SFLOAT,
			VK_FORMAT_R32G32B32A32_UINT,
			VK_FORMAT_R32G32B32A32_SINT,
			VK_FORMAT_R32G32B32A32_SFLOAT,
		};
		return table[static_cast<uint32>(format)];
	}

	VkBlendFactor ConvertToVKBlendFactor(BLEND_FACTOR factor)
	{
		VkBlendFactor table[] = {
			VK_BLEND_FACTOR_ZERO,
			VK_BLEND_FACTOR_ONE,
			VK_BLEND_FACTOR_SRC_COLOR,
			VK_BLEND_FACTOR_SRC_ALPHA,
			VK_BLEND_FACTOR_DST_COLOR,
			VK_BLEND_FACTOR_DST_ALPHA,
			VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
			VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
			VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
			VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
		};
		return table[static_cast<uint32>(factor)];
	}

	VkPolygonMode ConvertToVKFillMode(POLYGON_MODE mode)
	{
		VkPolygonMode table[] = {
			VK_POLYGON_MODE_FILL,
			VK_POLYGON_MODE_LINE,
		};
		return table[static_cast<uint32>(mode)];
	}

    VkCullModeFlagBits ConvertToVKCullMode(CULL_MODE_FLAG mode)
	{
		VkCullModeFlagBits table[] = {
			VK_CULL_MODE_NONE,
            VK_CULL_MODE_FRONT_BIT,
            VK_CULL_MODE_BACK_BIT,
		};
		return table[static_cast<uint32>(mode)];
	}

	VkBlendOp ConvertToVKBlendOp(BLEND_OP op)
	{
		VkBlendOp table[] = {
			VK_BLEND_OP_ADD,
			VK_BLEND_OP_SUBTRACT,
			VK_BLEND_OP_REVERSE_SUBTRACT,
			VK_BLEND_OP_MIN,
		};
		return table[static_cast<uint32>(op)];
	}

	VkLogicOp ConvertToVKLogicOp(LOGIC_OP op)
	{
		VkLogicOp table[] = {
			VK_LOGIC_OP_CLEAR,
			VK_LOGIC_OP_AND,
			VK_LOGIC_OP_AND_REVERSE,
			VK_LOGIC_OP_COPY,
			VK_LOGIC_OP_AND_INVERTED,
			VK_LOGIC_OP_NO_OP,
			VK_LOGIC_OP_XOR,
			VK_LOGIC_OP_OR,
			VK_LOGIC_OP_NOR,
			VK_LOGIC_OP_EQUIVALENT,
			VK_LOGIC_OP_INVERT,
			VK_LOGIC_OP_OR_REVERSE,
		};
		return table[static_cast<uint32>(op)];
	}

	uint8 ConvertToVKColorComponent(uint32 component)
	{
		uint8 ret = 0;
		if (component & static_cast<uint8>(COLOR_COMPONENT_FRAG::R)) ret |= VK_COLOR_COMPONENT_R_BIT;
		if (component & static_cast<uint8>(COLOR_COMPONENT_FRAG::G)) ret |= VK_COLOR_COMPONENT_G_BIT;
		if (component & static_cast<uint8>(COLOR_COMPONENT_FRAG::B)) ret |= VK_COLOR_COMPONENT_B_BIT;
		if (component & static_cast<uint8>(COLOR_COMPONENT_FRAG::A)) ret |= VK_COLOR_COMPONENT_A_BIT;
		return ret;
	}

	VkShaderStageFlagBits ConvertToVKShaderVisibility(SHADER_VISIBILITY visibility)
	{
		VkShaderStageFlagBits table[] = {
				VK_SHADER_STAGE_VERTEX_BIT,
				VK_SHADER_STAGE_FRAGMENT_BIT,
				VK_SHADER_STAGE_GEOMETRY_BIT,
				VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
				VK_SHADER_STAGE_ALL_GRAPHICS,
		};
		return table[static_cast<uint32>(visibility)];
	}

	VkDescriptorType ConvertToVKDescriptorType(DESCRIPTOR_TYPE type)
	{
		VkDescriptorType table[] = {
                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,

                VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,

				VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				VK_DESCRIPTOR_TYPE_SAMPLER,
		};
		return table[static_cast<uint32>(type)];
	}

	VkDescriptorType ConvertToVKDescriptorHeapType(DESCRIPTOR_HEAP_TYPE type)
	{
		VkDescriptorType table[] = {
				VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				VK_DESCRIPTOR_TYPE_SAMPLER,
				// TODO: RENDER_TARGET,
				// TODO: DEPTH_STENCIL,
		};
		return table[static_cast<uint32>(type)];
	}

}

#elif defined(_USE_DIRECTX12)
#include <d3d12.h>
#include <dxgi1_4.h>
#include "libs/DX12/d3dx12.h"

using revGraphicsDevice = ID3D12Device*;
using revGraphicsAdapter = IDXGIAdapter1*;
using revSwapChain = IDXGISwapChain3*;
using revShaderHandle = ID3DBlob*;
using revGraphicsResource = ID3D12Resource*;
using revGraphicsCommandAllocator = ID3D12CommandAllocator*;
using revGraphicsCommandBuffer = ID3D12GraphicsCommandList*;
using revGraphicsCommandQueue = ID3D12CommandQueue*;
using revGraphicsPipeline = ID3D12PipelineState*;
using revTextureHandle = ID3D12Resource*;
using revTextureSampler = D3D12_GPU_DESCRIPTOR_HANDLE;
using revDescriptorHeap = ID3D12DescriptorHeap;


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
			DXGI_FORMAT_R32_UINT,
			DXGI_FORMAT_R32_SINT,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_R32G32_UINT,
			DXGI_FORMAT_R32G32_SINT,
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_R32G32B32_UINT,
			DXGI_FORMAT_R32G32B32_SINT,
			DXGI_FORMAT_R32G32B32_FLOAT,
			DXGI_FORMAT_R32G32B32A32_UINT,
			DXGI_FORMAT_R32G32B32A32_SINT,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
		};
		return table[static_cast<uint32>(format)];
	}

	REV_INLINE D3D12_BLEND ConvertToDXBlendFactor(BLEND_FACTOR factor)
	{
		D3D12_BLEND table[] = {
			D3D12_BLEND_ZERO,
			D3D12_BLEND_ONE,
			D3D12_BLEND_SRC_COLOR,
			D3D12_BLEND_SRC_ALPHA,
			D3D12_BLEND_DEST_COLOR,
			D3D12_BLEND_DEST_ALPHA,
			D3D12_BLEND_INV_SRC_COLOR,
			D3D12_BLEND_INV_SRC_ALPHA,
			D3D12_BLEND_INV_DEST_COLOR,
			D3D12_BLEND_INV_DEST_ALPHA,
		};
		return table[static_cast<uint32>(factor)];
	}


	REV_INLINE D3D12_FILL_MODE ConvertToDXFillMode(POLYGON_MODE mode)
	{
		D3D12_FILL_MODE table[] = {
			D3D12_FILL_MODE_SOLID,
			D3D12_FILL_MODE_WIREFRAME,
		};
		return table[static_cast<uint32>(mode)];
	}

	REV_INLINE D3D12_CULL_MODE ConvertToDXCullMode(CULL_MODE_FLAG mode)
	{
		D3D12_CULL_MODE table[] = {
			D3D12_CULL_MODE_NONE,
			D3D12_CULL_MODE_FRONT,
			D3D12_CULL_MODE_BACK,
		};
		return table[static_cast<uint32>(mode)];
	}

	REV_INLINE D3D12_BLEND_OP ConvertToDXBlendOp(BLEND_OP op)
	{
		D3D12_BLEND_OP table[] = {
			D3D12_BLEND_OP_ADD,
			D3D12_BLEND_OP_SUBTRACT,
			D3D12_BLEND_OP_REV_SUBTRACT,
			D3D12_BLEND_OP_MIN,
		};
		return table[static_cast<uint32>(op)];
	}

	REV_INLINE D3D12_LOGIC_OP ConvertToDXLogicOp(LOGIC_OP op)
	{
		D3D12_LOGIC_OP table[] = {
			D3D12_LOGIC_OP_CLEAR,
			D3D12_LOGIC_OP_AND,
			D3D12_LOGIC_OP_AND_REVERSE,
			D3D12_LOGIC_OP_COPY,
			D3D12_LOGIC_OP_AND_INVERTED,
			D3D12_LOGIC_OP_NOOP,
			D3D12_LOGIC_OP_XOR,
			D3D12_LOGIC_OP_OR,
			D3D12_LOGIC_OP_NOR,
			D3D12_LOGIC_OP_EQUIV,
			D3D12_LOGIC_OP_INVERT,
			D3D12_LOGIC_OP_OR_REVERSE,
		};
		return table[static_cast<uint32>(op)];
	}

	REV_INLINE uint8 ConvertToDXColorComponent(uint8 component)
	{
		uint8 ret = 0;
		if (component & static_cast<uint8>(COLOR_COMPONENT_FRAG::R)) ret |= D3D12_COLOR_WRITE_ENABLE_RED;
		if (component & static_cast<uint8>(COLOR_COMPONENT_FRAG::G)) ret |= D3D12_COLOR_WRITE_ENABLE_GREEN;
		if (component & static_cast<uint8>(COLOR_COMPONENT_FRAG::B)) ret |= D3D12_COLOR_WRITE_ENABLE_BLUE;
		if (component & static_cast<uint8>(COLOR_COMPONENT_FRAG::A)) ret |= D3D12_COLOR_WRITE_ENABLE_ALPHA;
		return ret;
	}

	INPUT_ELEMENT_TYPE ConvertToREVSemantic(const char* name)
	{
		for (uint32 i = 0; i < static_cast<uint32>(INPUT_ELEMENT_TYPE::MAX_NUM); ++i) {
			if (strcmp(GRAPHICS_SEMANTICS[i].name, name) == 0) {
				return static_cast<INPUT_ELEMENT_TYPE>(i);
			}
		}
		return INPUT_ELEMENT_TYPE::MAX_NUM;
	}

	const char* ConvertToDXSemantic(INPUT_ELEMENT_TYPE type)
	{
		return GRAPHICS_SEMANTICS[static_cast<uint32>(type)].name;
	}

	uint32 ConvertToDXSemanticIndex(INPUT_ELEMENT_TYPE type)
	{
		return GRAPHICS_SEMANTICS[static_cast<uint32>(type)].index;
	}

	D3D12_SHADER_VISIBILITY ConvertToDXShaderVisibility(SHADER_VISIBILITY visibility)
	{
		D3D12_SHADER_VISIBILITY table[] = {
			D3D12_SHADER_VISIBILITY_VERTEX,
			D3D12_SHADER_VISIBILITY_PIXEL,
			D3D12_SHADER_VISIBILITY_GEOMETRY,
			D3D12_SHADER_VISIBILITY_HULL,
			D3D12_SHADER_VISIBILITY_ALL,
		};

		return table[static_cast<uint32>(visibility)];
	}

	D3D12_DESCRIPTOR_RANGE_TYPE ConverToDXDescriptorType(DESCRIPTOR_TYPE type)
	{
		D3D12_DESCRIPTOR_RANGE_TYPE table[] = {
			D3D12_DESCRIPTOR_RANGE_TYPE_SRV,		// TEXTURE_SHADER_RESOURCE_VIEW
			D3D12_DESCRIPTOR_RANGE_TYPE_SRV,		// RAW_BUFFER_SHADER_RESOURCE_VIEW
			D3D12_DESCRIPTOR_RANGE_TYPE_SRV,		// TYPED_BUFFER_SHADER_RESOURCE_VIEW
			D3D12_DESCRIPTOR_RANGE_TYPE_SRV,		// STRUCTURED_BUFFER_SHADER_RESOURCE_VIEW
			D3D12_DESCRIPTOR_RANGE_TYPE_UAV,		// TEXTURE_UNORDERED_ACCESS_VIEW
			D3D12_DESCRIPTOR_RANGE_TYPE_UAV,		// RAW_BUFFER_UNORDERED_RESOURCE_VIEW
			D3D12_DESCRIPTOR_RANGE_TYPE_UAV,		// TYPED_BUFFER_UNORDERED_RESOURCE_VIEW
			D3D12_DESCRIPTOR_RANGE_TYPE_UAV,		// STRUCTURED_BUFFER_UNORDERED_RESOURCE_VIEW
			D3D12_DESCRIPTOR_RANGE_TYPE_CBV,		// CONSTANT_BUFFER_VIEW
			D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER,	// SAMPLER
		};
		return table[static_cast<uint32>(type)];
	}

	D3D12_DESCRIPTOR_HEAP_TYPE ConvertToDXDescriptorHeapType(DESCRIPTOR_HEAP_TYPE type)
	{
		D3D12_DESCRIPTOR_HEAP_TYPE table[] = {
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
			D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
			D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
		};
		return table[static_cast<uint32>(type)];
	}

	SHADER_INPUT_TYPE ConvertToRevShaderInputType(_D3D_SHADER_INPUT_TYPE type)
	{
		switch (type) {
		case D3D_SIT_CBUFFER: return SHADER_INPUT_TYPE::CBUFFER;
		case D3D_SIT_TBUFFER: return SHADER_INPUT_TYPE::TBUFFER;
		case D3D_SIT_TEXTURE: return SHADER_INPUT_TYPE::TEXTURE;
		case D3D_SIT_SAMPLER: return SHADER_INPUT_TYPE::SAMPLER;
		case D3D_SIT_UAV_RWTYPED: return SHADER_INPUT_TYPE::UAV_RWTYPED;
		case D3D_SIT_STRUCTURED: return SHADER_INPUT_TYPE::STRUCTURED;
		case D3D_SIT_UAV_RWSTRUCTURED: return SHADER_INPUT_TYPE::UAV_RWSTRUCTURED;
		case D3D_SIT_BYTEADDRESS: return SHADER_INPUT_TYPE::BYTEADDRESS;
		case D3D_SIT_UAV_RWBYTEADDRESS: return SHADER_INPUT_TYPE::UAV_RWBYTEADDRESS;
		case D3D_SIT_UAV_APPEND_STRUCTURED: return SHADER_INPUT_TYPE::UAV_APPEND_STRUCTURED;
		case D3D_SIT_UAV_CONSUME_STRUCTURED: return SHADER_INPUT_TYPE::UAV_CONSUME_STRUCTURED;
		case D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER: return SHADER_INPUT_TYPE::UAV_RWSTRUCTURED_WITH_COUNTER;
		}
		return SHADER_INPUT_TYPE::MAX_NUM;
	}


}


#endif


#endif
