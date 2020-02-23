#ifndef __VULKANSHADER_H__
#define __VULKANSHADER_H__
#include "VulkanDeviceContext.h"
#include "revString.h"

enum class SHADER_TYPE : uint32
{
    VERTX,
    FRAGMENT,
    COMPUTE,
};

class VulkanShader
{
public:
    VulkanShader();
    virtual ~VulkanShader(){}

    bool LoadFromFile(const VulkanDeviceContext& deviceContext, const char* path, SHADER_TYPE shaderType);
private:
    static const uint32 SHADER_TYPE_MAX_NUM = 3;
    revString name;
    VkShaderModule* handle[SHADER_TYPE_MAX_NUM];
};

#endif

