#ifdef _USE_VULKAN

#include "VulkanShader.h"
#include "File.h"
#include "Log.h"


VulkanShader::VulkanShader()
{
    handle = VK_NULL_HANDLE;
}


bool VulkanShader::LoadFromFile(const revDevice& device, const char* path, SHADER_TYPE shaderType)
{
    revString resourcePath(RESOURCE_PATH);
    resourcePath += path;
    File file;
    if(!file.Open(resourcePath.c_str(), FileMode::ReadText)){
        NATIVE_LOGE("file not found : %s", path);
        return false;
    }

    type = shaderType;
    uint32 length = file.GetFileSize();
    char* data = new char[length];
    file.ReadData(data);

    VkShaderModuleCreateInfo shaderModuleCreateInfo {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = nullptr,
        .codeSize = length,
        .pCode = (const uint32*)data,
        .flags = 0
    };
    VkResult result = vkCreateShaderModule(device.GetDevice(), &shaderModuleCreateInfo, nullptr, &handle);
    delete[] data;
    if(result != VK_SUCCESS){
        NATIVE_LOGE("Vulkan error. File[%s], line[%d]", __FILE__,__LINE__);
        return false;
    }
    return LoadMetaData(resourcePath.c_str());
}

VkPipelineShaderStageCreateInfo VulkanShader::getShaderStageCreateInfo() const
{
    VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = nullptr,
        .module = handle,
        .pSpecializationInfo = nullptr,
        .flags = 0,
        .pName = "main",
    };
    const VkShaderStageFlagBits SHADER_STAGE_FLAG[] = {
            VK_SHADER_STAGE_VERTEX_BIT,
            VK_SHADER_STAGE_FRAGMENT_BIT,
    };
    pipelineShaderStageCreateInfo.stage = SHADER_STAGE_FLAG[static_cast<int32>(type)];
    return pipelineShaderStageCreateInfo;
}

#endif