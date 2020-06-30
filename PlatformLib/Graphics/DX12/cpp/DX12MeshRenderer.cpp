#ifdef _USE_DIRECTX12
#include "DX12MeshRenderer.h"
#include "revGraphics.h"

DX12MeshRenderer::DX12MeshRenderer()
{
}

DX12MeshRenderer::~DX12MeshRenderer()
{

}

void DX12MeshRenderer::Initialize()
{
	const uint32 vertexBufferCount = static_cast<uint32>(vertexBuffers.size());
	vertexBufferViews.resize(vertexBufferCount);
	for (uint32 i = 0; i < vertexBufferCount; ++i) {
		vertexBufferViews[i] = new DX12VertexBufferView();
		vertexBufferViews[i]->Create(revGraphics::Get().GetDevice(), vertexBuffers[i]);
	}

	indexBufferViews.resize(vertexBufferCount);
	for (uint32 i = 0; i < vertexBufferCount; ++i) {
		if (indexBuffers[i] != nullptr) {
			indexBufferViews[i] = new DX12IndexBufferView();
			indexBufferViews[i]->Create(revGraphics::Get().GetDevice(), indexBuffers[i]);
		}
	}
}

void DX12MeshRenderer::Finalize()
{
	const uint32 vertexBufferCount = static_cast<uint32>(vertexBuffers.size());
	for (uint32 i = 0; i < vertexBufferCount; ++i) {
		vertexBufferViews[i]->Destroy();
		delete vertexBufferViews[i];
		vertexBufferViews[i] = nullptr;

		if (indexBufferViews[i] != nullptr) {
			indexBufferViews[i]->Destroy();
			delete indexBufferViews[i];
			indexBufferViews[i] = nullptr;
		}
	}
	vertexBufferViews.clear();
}

void DX12MeshRenderer::Draw(revGraphicsCommandList& commandList)
{
	auto& list = commandList.GetList();

	list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (uint32 i = 0; i < static_cast<uint32>(vertexBufferViews.size()); ++i) {
		list->IASetVertexBuffers(0, 1, vertexBufferViews[i]->GetResourceView());
		if (indexBufferViews.size() > i && indexBufferViews[i] != nullptr) {
			list->IASetIndexBuffer(indexBufferViews[i]->GetResourceView());
			list->DrawIndexedInstanced(indexBufferViews[i]->GetCount() * 3, 1, 0, 0, 0);
		}
		else {
			list->DrawInstanced(vertexBuffers[i]->GetLength(), 1, 0, 0);
		}
	}
}



#endif
