#ifndef __VULKANSHADER_H__
#define __VULKANSHADER_H__

enum SHADER_TYPE{
    SHADER_TYPE_VERTX,
    SHADER_TYPE_FRAGMENT,
    SHADER_TYPE_COMPUTE,
    SHADER_TYPE_MAX_NUM,
};

class VulkanShader
{
public:
    VulkanShader(){}
    virtual ~VulkanShader(){}

    bool LoadFromFile(const char& path, SHADER_TYPE shaderType);
private:


};

#endif

