#ifndef __VKRENDERINFO_H__
#define __VKRENDERINFO_H__

#include "VkDeviceContext.h"
#include "VkSwapChain.h"

class VkRenderInfo
{
public:
    bool Create(const VkDeviceContext& deviceContext, const VkSwapChain& swapChain);
    void Destroy();

private:
    VkRenderPass renderPass;
    //VkCommandPool  cmdPool;
    //VkCommandBuffer* cmdBuffer;
    //uint32 cmdBufferLength;
    //VkSemaphore  semaphore;
    //VkFence fence;
};

#endif
