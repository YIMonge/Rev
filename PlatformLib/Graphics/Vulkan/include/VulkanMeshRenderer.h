#ifndef __VULKANMESHRENDERER_H__
#define __VULKANMESHRENDERER_H__
#ifdef _USE_VULKAN

#include "revMeshRenderer.h"
#include "VulkanBuffer.h"

class VulkanMeshRenderer : public revMeshRenderer
{
public:
	VulkanMeshRenderer();
	virtual ~VulkanMeshRenderer();

	virtual void Draw(revGraphicsCommandList& commandList);

};

#endif
#endif
