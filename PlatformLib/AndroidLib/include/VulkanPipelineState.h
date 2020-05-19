#ifndef __VULKANPIPELINESTATE_H__
#define __VULKANPIPELINESTATE_H__

#include "VulkanDevice.h"
#include "revMaterial.h"
#include "VulkanCommandList.h"
#include "VulkanDescriptorSet.h"
#include "revRect.h"
#include "VulkanRenderPass.h"

struct PipelineStateDesc
{
    revMaterial* material;
    VulkanDescriptorSetLayout* descriptorSetLayout;
    VulkanRenderPass* renderPass;
    revRect viewportRect;
    revRect scissorRect;
};

class VulkanPipelineState
{
public:
    VulkanPipelineState() {}
    virtual ~VulkanPipelineState(){}

    bool Create(revDevice* device, const PipelineStateDesc& desc);
    void Destroy();
    void Apply(VulkanCommandList& commandList);

    const revRect& GetViewport() const { return viewportRect; }
    const VkPipelineLayout& GetPipelineLayout() const { return pipelineLayout; }

private:
    revDevice* device;
    VkPipelineCache pipelineCache;
    revGraphicsPipeline pipelineState;
    VkPipelineLayout pipelineLayout;
    revRect viewportRect;
    revRect scissorRect;
};

#endif
