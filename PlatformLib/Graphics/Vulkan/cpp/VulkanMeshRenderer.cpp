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
	uint32 vertexBufferCount = static_cast<uint32>(vertexBuffers.size());
	VkDeviceSize offset = 0;
	for (uint32 i = 0; i < vertexBufferCount; ++i) {
		vkCmdBindVertexBuffers(list, 0, 1, vertexBuffers[i]->GetHandlePtr(), &offset);
		if (indexBuffers[i] != nullptr) {
			vkCmdBindIndexBuffer(list, *(indexBuffers[i]->GetHandlePtr()), 0, VK_INDEX_TYPE_UINT32);
			vkCmdDrawIndexed(list, indexBuffers[i]->GetLength() * 3, 1, 0, 0, 0);
		}
		else {
			vkCmdDraw(list, vertexBuffers[i]->GetLength(), 1, 0, 0);
		}
	}
}


#endif