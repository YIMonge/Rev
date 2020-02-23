#ifndef __VULKANSHADER_H__
#define __VULKANSHADER_H__
#include "VulkanDeviceContext.h"
#include "revString.h"

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
    VkShaderModule* getShader(){
        return handle;
    }
private:
    revString name;
    SHADER_TYPE  type;
    VkShaderModule* handle;
};

// TODO:
class VulkanComputeShader
{

};


#endif

