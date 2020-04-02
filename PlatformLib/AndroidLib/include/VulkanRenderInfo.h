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

    bool Create(const VulkanDevice& device, const VulkanSwapChain& swapChain);
    void Destroy(const VulkanDevice& device);

    bool CreatePipeline(const VulkanDevice& device, const VulkanSwapChain& swapChain, const VulkanShader& vertexShader, const VulkanShader& fragmentShader);
    bool CreatePipeline(const VulkanDevice& device, const VulkanSwapChain& swapChain, const revMaterial* material);

    bool CreateDescriptorSet(const VulkanDevice& device, VulkanTexture& texture);

    const VkRenderPass& GetRenderPass() const { return renderPass; }
    const VkSemaphore& GetSemaphore() const { return semaphore; }
    const VkFence& GetFence() const { return fence; }
    const VkPipeline& GetPipeline() const { return pipeline; }
private:


    VkRenderPass renderPass;
    VkSemaphore  semaphore;
    VkFence fence;

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
