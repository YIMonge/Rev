#ifndef __VULKANSHADER_H__
#define __VULKANSHADER_H__
#include "revString.h"
#include "IShader.h"

#ifdef _USE_VULKAN

class VulkanShader : public IShader
{
public:
    VulkanShader();
    virtual ~VulkanShader(){}

    virtual bool LoadFromFile(const IDeviceContext& deviceContext, const char* path, SHADER_TYPE shaderType);
    VkPipelineShaderStageCreateInfo getShaderStageCreateInfo() const;
};

// TODO:
class VulkanComputeShader
{

};


#endif
#endif

