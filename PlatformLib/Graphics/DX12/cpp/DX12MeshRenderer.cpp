#ifdef _USE_DIRECTX12
#include "DX12MeshRenderer.h"
#include "revGraphics.h"

DX12MeshRenderer::DX12MeshRenderer()
{
}

DX12MeshRenderer::~DX12MeshRenderer()
{

}

void DX12MeshRenderer::SetModel(const revModel* model)
{
	revMeshRenderer::SetModel(model);

	uint32 transformCount = static_cast<uint32>(transforms.size());
	constantBufferViews.resize(transformCount, nullptr);
	for (uint32 i = 0; i < transformCount; ++i) {
		constantBufferViews[i] = new DX12ConstantBufferView();
	}
}

void DX12MeshRenderer::SetMesh(uint32 index, const revMesh& mesh)
{
	revMeshRenderer::SetMesh(index, mesh);

	if (vertexBufferViews.size() <= index) {
		vertexBufferViews.resize(index + 1);
	}
	vertexBufferViews[index] = new DX12VertexBufferView();
	vertexBufferViews[index]->Create(revGraphics::Get().GetDevice(), vertexBuffers[index]);

	if (indexBufferViews.size() <= index) {
		indexBufferViews.resize(index + 1);
	}
	if (indexBuffers[index] != nullptr) {
		indexBufferViews[index] = new DX12IndexBufferView();
		indexBufferViews[index]->Create(revGraphics::Get().GetDevice(), indexBuffers[index]);
	}
}

void DX12MeshRenderer::Destroy()
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

void DX12MeshRenderer::Initialize(DX12DescriptorHeap* cBufferHeap)
{
	uint32 cbufferviewCount = static_cast<uint32>(constantBufferViews.size());
	cbufferHeapChunk = cBufferHeap->Allocation(cbufferviewCount);
	for (uint32 i = 0; i < cbufferviewCount; ++i) {
		constantBufferViews[i]->Create(revGraphics::Get().GetDevice(), constantBuffers[i], cbufferHeapChunk.GetHandle(i));
	}
}

void DX12MeshRenderer::Draw(revGraphicsCommandList& commandList, DX12DescriptorHeap& cBufferHeap, DX12DescriptorHeap& textureHeap, DX12DescriptorHeap& samplerHeap)
{
	auto& list = commandList.GetList();

	list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (uint32 i = 0; i < static_cast<uint32>(vertexBufferViews.size()); ++i) {
		if (constantBufferViews[i] != nullptr) {
			cBufferHeap.Apply(commandList, 0, cbufferHeapChunk.GetDescriptorOffset(i));
		}
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
