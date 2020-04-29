#include "VulkanPipelineState.h"

bool VulkanPipelineState::Create(revDevice* device, const revMaterial& material)
{
    this->device = device;

    return true;
}

void VulkanPipelineState::Apply(VulkanCommandList& commandList)
{

}