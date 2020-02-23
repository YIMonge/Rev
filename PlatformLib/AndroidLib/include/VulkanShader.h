#ifndef __VULKANSHADER_H__
#define __VULKANSHADER_H__
#include "VulkanDeviceContext.h"

enum class SHADER_TYPE{
    VERTX,
    FRAGMENT,
    COMPUTE,
    MAX_NUM,
};

class VulkanShader
{
public:
    VulkanShader(){}
    virtual ~VulkanShader(){}

    bool LoadFromFile(const VulkanDeviceContext& deviceContext, const char* path, SHADER_TYPE shaderType);
private:
    VkShaderModule* handle;
};

#endif

