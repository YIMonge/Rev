#include "VulkanShader.h"
#include "File.h"
#include "Log.h"

VulkanShader::VulkanShader() :
name("")
{
}



bool VulkanShader::LoadFromFile(const VulkanDeviceContext& deviceContext, const char* path, SHADER_TYPE shaderType)
{
    File file;
    if(!file.Open(path, FileMode::Mode::ReadText)){
        NATIVE_LOGE("file not found : %s", path);
        return false;
    }

    type = shaderType;
    uint32 length = file.GetFileSize();
    char* data = new char[length];
    file.GetData(data);

    VkShaderModuleCreateInfo shaderModuleCreateInfo {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = nullptr,
        .codeSize = length,
        .pCode = (const uint32*)data,
        .flags = 0
    };
    VkResult result = vkCreateShaderModule(deviceContext.GetDevice(), &shaderModuleCreateInfo,
                                           nullptr, handle);
    delete[] data;
    if(result != VK_SUCCESS){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    return true;
}