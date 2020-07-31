#ifndef __VULKANSHADER_H__
#define __VULKANSHADER_H__
#ifdef _USE_VULKAN

#include "revString.h"
#include "revShader.h"

class VulkanShader : public revShader
{
public:
    VulkanShader();
    virtual ~VulkanShader(){}

    virtual bool LoadFromFile(const revDevice* device, const char* path, SHADER_TYPE shaderType);
    VkPipelineShaderStageCreateInfo getShaderStageCreateInfo() const;
};

// TODO:
class VulkanComputeShader
{

};


#endif
#endif

