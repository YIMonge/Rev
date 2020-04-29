#ifndef __VULKANRENDERINFO_H__
#define __VULKANRENDERINFO_H__

#ifdef _USE_VULKAN

#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanShader.h"
#include "VulkanTexture.h"
#include "revMaterial.h"
#include "revMap.h"

class VulkanRenderInfo
{
public:
    VulkanRenderInfo();
    ~VulkanRenderInfo();

    bool Create(VulkanDevice* device, const VulkanSwapChain& swapChain);
    void Destroy(VulkanDevice* device);

    bool CreatePipeline(VulkanDevice* device, const VulkanSwapChain& swapChain, const VulkanShader& vertexShader, const VulkanShader& fragmentShader);
    bool CreatePipeline(VulkanDevice* device, const VulkanSwapChain& swapChain, const revMaterial* material);

    bool CreateDescriptorSet(VulkanDevice* device, VulkanTexture& texture);

    const VkRenderPass& GetRenderPass() const { return renderPass; }
    const VkPipeline& GetPipeline() const { return pipeline; }
private:
    VkRenderPass renderPass;


    // pipeline
    revUnorderedMap<uint32, VkPipelineCache> pipelineCaches;
    VkPipelineLayout pipelineLayout;
    VkPipelineCache pipelineCache;
    VkPipeline  pipeline;

    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSet;
    VkDescriptorSetLayout  descriptorSetLayout;

};

#endif
#endif
