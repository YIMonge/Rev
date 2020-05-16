#ifndef __VULKANPIPELINESTATE_H__
#define __VULKANPIPELINESTATE_H__

#include "VulkanDevice.h"
#include "revMaterial.h"
#include "VulkanCommandList.h"
#include "VulkanDescriptorSet.h"
#include "revRect.h"

struct PipelineStateDesc
{
    revMaterial* material;
    VulkanDescriptorSet* descriptorSet;
    revRect viewportRect;
    revRect scissorRect;
};

class VulkanPipelineState
{
public:
    VulkanPipelineState() {}
    virtual ~VulkanPipelineState(){}

    bool Create(revDevice* device, const revMaterial& material, const VulkanDescriptorSetLayout& descriptorSetLayout, const revRect& viewportRect, const revRect& scissorRect);
    void Destroy();
    void BeginRenderPass(VulkanCommandList& commandList, const VkFramebuffer& frameBuffer, const revColor& clearColor);
    void EndRenderPass(VulkanCommandList& commandList);

    const VkRenderPass& GetRenderPass() const { return renderPass; }
    const VkPipelineLayout& GetPipelineLayout() const { return pipelineLayout; }

private:
    revDevice* device;
    VkPipelineCache pipelineCache;
    revGraphicsPipeline pipelineState;
    VkPipelineLayout pipelineLayout;
    VkRenderPass renderPass;
    revRect viewportRect;
    revRect scissorRect;
};

#endif
