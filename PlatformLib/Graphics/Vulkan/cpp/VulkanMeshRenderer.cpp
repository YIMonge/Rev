#ifdef _USE_VULKAN
#include "VulkanMeshRenderer.h"

VulkanMeshRenderer::VulkanMeshRenderer()
{
}

VulkanMeshRenderer::~VulkanMeshRenderer()
{

}

void VulkanMeshRenderer::Draw(revGraphicsCommandList& commandList)
{
	auto& list = commandList.GetList();
	uint32 drawCount = static_cast<uint32>(drawResources.size());
	for (uint32 i = 0; i < drawCount; ++i) {
		if (drawResources[i] == nullptr) continue;
		if (drawResources[i]->transformIndex != DONT_HAVE_CONSTANT_BUFFER) {
			// TODO: apply transform cbuffer
		}
		if (drawResources[i]->materialIndex != DONT_HAVE_CONSTANT_BUFFER) {
			// TODO: apply material cbuffer
		}

		if (drawResources[i]->indexBuffer != nullptr) {
			vkCmdBindIndexBuffer(list, *(drawResources[i]->indexBuffer->GetHandlePtr()), 0, VK_INDEX_TYPE_UINT32);
			vkCmdDrawIndexed(list, drawResources[i]->indexBuffer->GetLength() * 3, 1, 0, 0, 0);
		}
		else {
			vkCmdDraw(list, drawResources[i]->vertexBuffer->GetLength(), 1, 0, 0);
		}
	}
}


#endif