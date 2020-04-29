#include "VulkanPipelineState.h"
#include "revArray.h"

bool VulkanPipelineState::Create(revDevice *device, const revMaterial &material)
{
    this->device = device;
    auto& vkDevice = device->GetDevice();
    const revShader* vertexShader = material.GetShader(SHADER_TYPE::COMPUTE);
    const revShader* fragmentShader = material.GetShader(SHADER_TYPE::FRAGMENT);



    return true;
}

void VulkanPipelineState::Apply(VulkanCommandList &commandList)
{

}