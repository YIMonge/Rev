#ifndef __GRAPHICS_TYPEDEF_H__
#define __GRAPHICS_TYPEDEF_H__

#include "revTypedef.h"
#include "revMath.h"

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

enum class REV_GRAPHICS_BUFFER_FOMAT_FLAG : uint32
{
    VERTEX      = 0x00000001,
    NORMAL      = 0x00000002,
    TEXCOORD_0  = 0x00000004,
    TEXCOORD_1  = 0x00000008,
    COLOR_0     = 0x00000010,
    COLOR_1     = 0x00000020,
    COLOR_2     = 0x00000040,
    COLOR_3     = 0x00000080,
    VECTOR2_0   = 0x00000100,
    VECTOR2_1   = 0x00000200,
    VECTOR2_2   = 0x00000400,
    VECTOR2_3   = 0x00000800,
    VECTOR3_0   = 0x00001000,
    VECTOR3_1   = 0x00002000,
    VECTOR3_2   = 0x00004000,
    VECTOR3_3   = 0x00008000,
    VECTOR4_0   = 0x00010000,
    VECTOR4_1   = 0x00020000,
    VECTOR4_2   = 0x00040000,
    VECTOR4_3   = 0x00080000,

    MAX = 0xffffffff,
};

static uint32 GetStrideSize(REV_GRAPHICS_BUFFER_FOMAT_FLAG format) {
    const uint32 strideSize[] = {
            sizeof(revVector3), // VERTEX
            sizeof(revVector3), // NORMAL
            sizeof(revVector2), // TEXCOORD_0
            sizeof(revVector2), // TEXCOORD_1
            sizeof(revVector4), // COLOR_0
            sizeof(revVector4), // COLOR_1
            sizeof(revVector4), // COLOR_2
            sizeof(revVector4), // COLOR_3
            sizeof(revVector2), // VECTOR2_0
            sizeof(revVector2), // VECTOR2_1
            sizeof(revVector2), // VECTOR2_2
            sizeof(revVector2), // VECTOR2_3
            sizeof(revVector3), // VECTOR3_0
            sizeof(revVector3), // VECTOR3_1
            sizeof(revVector3), // VECTOR3_2
            sizeof(revVector3), // VECTOR3_3
            sizeof(revVector4), // VECTOR4_0
            sizeof(revVector4), // VECTOR4_1
            sizeof(revVector4), // VECTOR4_2
            sizeof(revVector4), // VECTOR4_3
    };

    uint32 ret = 0;
    for(uint32 i = 0; i < 32; ++i){
        if((static_cast<uint32>(format) & (1 << i)) == 1) ret += strideSize[i];
    }
    return ret;
}

// belows are specific graphics lib arguments.
#ifdef _USE_VULKAN
#include "../lib/vulkan_wrapper.h"
using revGraphicsDevice = VkDevice;
using revSwapChain = VkSwapchainKHR;
using revShaderHandle = VkShaderModule;
using revGraphicsResource = VkBuffer;
using revGraphicsCommandBuffer = VkCommandBuffer;
using revGraphicsCommandQueue = VkQueue;
using revTextureResourceView = VkImageView;
using revTextureSampler = VkSampler;


// Formats
using revGraphicsFormat = VkFormat;

#define REV_GRAPHICS_FORMAT_R8G8B8A8_UNORM VK_FORMAT_R8G8B8A8_UNORM

enum class COLOR_COMPONENT_FRAG
{
    R = VK_COLOR_COMPONENT_R_BIT,
    G = VK_COLOR_COMPONENT_G_BIT,
    B = VK_COLOR_COMPONENT_B_BIT,
    A = VK_COLOR_COMPONENT_A_BIT,
    ALL = R | G| B | A
};

enum class BLEND_OP
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
    POINT = VK_POLYGON_MODE_POINT,
};

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

using revTextureResourceView = D3D12_CPU_DESCRIPTOR_HANDLE;
//using revTextureSampler = // TODO:

using revGraphicsFormat = DXGI_FORMAT;

const revGraphicsFormat REV_GRAPHICS_FORMAT_R8G8B8A8_UNORM = DXGI_FORMAT_B8G8R8A8_UNORM;

#endif


#endif
