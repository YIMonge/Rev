#ifndef __VULKANSHADER_H__
#define __VULKANSHADER_H__
#include "VulkanDeviceContext.h"
#include "revString.h"

#ifdef _USE_VULKAN

enum class SHADER_TYPE : uint32
{
    VERTX,
    FRAGMENT,
};

class VulkanShader
{
public:
    VulkanShader();
    virtual ~VulkanShader(){}

    bool LoadFromFile(const VulkanDeviceContext& deviceContext, const char* path, SHADER_TYPE shaderType);
    VkPipelineShaderStageCreateInfo getShaderStageCreateInfo() const;

private:
    revString name;
    SHADER_TYPE  type;
    VkShaderModule handle;
};

// TODO:
class VulkanComputeShader
{

};


#endif
#endif

