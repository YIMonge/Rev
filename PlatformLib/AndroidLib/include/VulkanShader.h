#ifndef __VULKANSHADER_H__
#define __VULKANSHADER_H__
#include "revString.h"
#include "revShader.h"

#ifdef _USE_VULKAN

class VulkanShader : public revShader
{
public:
    VulkanShader();
    virtual ~VulkanShader(){}

    virtual bool LoadFromFile(const revDevice& device, const char* path, SHADER_TYPE shaderType);
    VkPipelineShaderStageCreateInfo getShaderStageCreateInfo() const;
};

// TODO:
class VulkanComputeShader
{

};


#endif
#endif

