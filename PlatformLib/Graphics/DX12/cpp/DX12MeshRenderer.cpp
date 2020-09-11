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
	revDevice* device = revGraphics::Get().GetDevice();
	uint32 transformCount = static_cast<uint32>(transforms.size());
	transformConstantBufferViews.resize(transformCount, nullptr);

	/* TODO: upload 
	for (uint32 i = 0; i < transformCount; ++i) {
		transformConstantBufferViews[i] = new DX12ConstantBufferView(device);
	}
	*/
}

void DX12MeshRenderer::SetMesh(uint32 index, const revMesh* mesh)
{
	revMeshRenderer::SetMesh(index, mesh);

	if (drawResources[index] == nullptr || drawResources[index]->vertexBuffer == nullptr) return;

	revDevice* device = revGraphics::Get().GetDevice();

	if (vertexBufferViews.size() <= index) {
		vertexBufferViews.resize(index + 1);
	}

	/*
	vertexBufferViews[index] = new DX12VertexBufferView(device);
	vertexBufferViews[index]->Create(drawResources[index]->vertexBuffer);

	if (indexBufferViews.size() <= index) indexBufferViews.resize(index + 1);
	
	if (drawResources[index]->indexBuffer != nullptr) {
		indexBufferViews[index] = new DX12IndexBufferView(device);
		indexBufferViews[index]->Create(drawResources[index]->indexBuffer);
	}
	*/	
}

/// <summary>
/// destroy all resources in this renderer.
/// </summary>
void DX12MeshRenderer::Destroy()
{
	const uint32 vertexBufferCount = static_cast<uint32>(drawResources.size());
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
	


	revMeshRenderer::Destroy();
}

/// <summary>
/// 
/// </summary>
/// <param name="camera"></param>
void DX12MeshRenderer::PrepareDraw(const revCamera& camera)
{
	revTransform::CBuffer cbuffer;
	revMatrix44 viewProj = camera.GetViewMatrix() * camera.GetProjectionMatrix();

	uint32 cbufferviewCount = static_cast<uint32>(transformConstantBufferViews.size());
	for (uint32 i = 0; i < cbufferviewCount; ++i) {
		cbuffer.world = transforms[i]->GetWorldMatrix();
		cbuffer.wvp = cbuffer.world * viewProj;
		cbuffer.world.Transpose();
		cbuffer.wvp.Transpose();

		transformConstantBuffers[i]->Update(&cbuffer, sizeof(cbuffer));
	}
}


void DX12MeshRenderer::Draw(revGraphicsCommandList& commandList, DX12DescriptorHeap& cBufferHeap, DX12DescriptorHeap& textureHeap, DX12DescriptorHeap& samplerHeap)
{
	auto& list = commandList.GetList();

	list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	uint32 drawCount = static_cast<uint32>(drawResources.size());
	for (uint32 i = 0; i < drawCount; ++i) {
		if (drawResources[i] == nullptr) continue;
		if (drawResources[i]->transformIndex != DONT_HAVE_CONSTANT_BUFFER) {
			//cBufferHeap.Apply(commandList, 0, transformCbufferHeapChunk->GetOffset(drawResources[i]->transformIndex));
			cBufferHeap.Apply(commandList, 0, transformConstantBufferViews[i]->GetOffset());
		}
		if (drawResources[i]->materialIndex != DONT_HAVE_CONSTANT_BUFFER) {
			//cBufferHeap.Apply(commandList, 0, materialCbufferHeapChunk->GetOffset(drawResources[i]->materialIndex));
		}
		list->IASetVertexBuffers(0, 1, vertexBufferViews[i]->GetResourceView());
		if (drawResources[i]->indexBuffer != nullptr) {
			list->IASetIndexBuffer(indexBufferViews[i]->GetResourceView());
			list->DrawIndexedInstanced(indexBufferViews[i]->GetCount() * 3, 1, 0, 0, 0);
		}
		else {
			list->DrawInstanced(drawResources[i]->vertexBuffer->GetLength(), 1, 0, 0);
		}
	}
}



#endif
