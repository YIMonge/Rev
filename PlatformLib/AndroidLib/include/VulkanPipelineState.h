#ifndef __VULKANPIPELINESTATE_H__
#define __VULKANPIPELINESTATE_H__

#include "VulkanDevice.h"
#include "revMaterial.h"
#include "VulkanCommandList.h"

class VulkanPipelineState
{
public:
    VulkanPipelineState() {}
    virtual ~VulkanPipelineState(){}

    bool Create(revDevice* device, const revMaterial& material);
    void Apply(VulkanCommandList& commandList);

private:
    revDevice* device;
    revGraphicsPipeline pipelineState;
};

#endif
