#ifndef __GRAPHICS_TYPEDEF_H__
#define __GRAPHICS_TYPEDEF_H__

#include "revTypedef.h"

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
};


#ifdef _USE_VULKAN
#include "../lib/vulkan_wrapper.h"
using revGraphicsDevice = VkDevice;
using revSwapChain = VkSwapchainKHR;
using revShaderHandle = VkShaderModule;
using revGraphicsResource = VkBuffer;
using revGraphicsFormat = VkFormat;

// Formats
const revGraphicsFormat REV_GRAPHICS_FORMAT_R8G8B8A8_UNORM = VK_FORMAT_R8G8B8A8_UNORM;

using revGraphicsCommandBuffer = VkCommandBuffer;
using revGraphicsCommandQueue = VkQueue;

#elif defined(_USE_DIRECTX12)
#include <d3d12.h>
#include <dxgi1_4.h>
#include "libs/DX12/d3dx12.h"
using revGraphicsDevice = ID3D12Device*;
using revSwapChain = IDXGISwapChain3;
using revShaderHandle = ID3DBlob;
using revGraphicsResource = ID3D12Resource;
using revGraphicsFormat = DXGI_FORMAT;

using revGraphicsCommandBuffer = ID3D12GraphicsCommandList;
using revGraphicsCommandQueue = ID3D12CommandQueue;

#endif


#endif
