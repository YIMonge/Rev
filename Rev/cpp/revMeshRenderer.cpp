#include "revMeshRenderer.h"
#include "revGraphics.h"
#include "Log.h"

revMeshRenderer::revMeshRenderer()
{
}

revMeshRenderer::~revMeshRenderer()
{
}

void revMeshRenderer::SetModel(const revModel* model)
{
	this->model = model;
	transforms = revArray<revTransform*>(model->GetTransforms());
	uint32 count = static_cast<uint32>(transforms.size());
	transformConstantBuffers.resize(count);

	revGraphics& graphicsMgr = revGraphics::Get();
	for (uint32 i = 0; i < count; ++i) {
		transformConstantBuffers[i] = graphicsMgr.CreateConstantBuffer();
		transformConstantBuffers[i]->Create(nullptr, sizeof(revTransform::CBuffer), 1, revGraphicsBuffer::USAGE::DYNAMIC);
		graphicsMgr.UploadResource(transformConstantBuffers[i]);
	}

	SetMeshes(model->GetMeshes());
}

void revMeshRenderer::SetMeshes(const revArray<revMesh*>& meshes)
{
	for (uint32 i = 0; i < static_cast<uint32>(meshes.size()); ++i) {
		SetMesh(i, meshes[i]);
	}
}

void revMeshRenderer::SetMesh(uint32 index, const revMesh* mesh)
{
	revGraphics& graphicsMgr = revGraphics::Get();
	if (static_cast<uint32>(drawResources.size()) <= index) {
		drawResources.resize(index + 1, nullptr);
	}
	DrawResources* drawResource = new DrawResources();
	drawResources[index] = drawResource;


	drawResource->vertexBuffer = graphicsMgr.CreateVertexBuffer();
	if (drawResource->vertexBuffer == nullptr) {
		NATIVE_LOGE("failed create vertex buffer. file:%s , line:%s", __FILE__, __LINE__);
	}

	const auto& vertices = mesh->GetVertexData();
	const uint32 sizeOfBytes = mesh->GetSizeOfBytes();
	drawResource->vertexBuffer->Create(vertices.data(), sizeOfBytes, static_cast<uint32>(vertices.size()) / (sizeOfBytes / sizeof(f32)));
	graphicsMgr.UploadResource(drawResource->vertexBuffer);

	if (mesh->GetIndexArray().size() > 0) {

		drawResource->indexBuffer = revGraphics::Get().CreateIndexBuffer();
		if (drawResource->indexBuffer == nullptr) {
			NATIVE_LOGE("failed create index buffer. file:%s , line:%s", __FILE__, __LINE__);
		}
		const auto& indicies = mesh->GetIndexArray();
		drawResource->indexBuffer->Create(indicies.data(), sizeof(revIndex3), static_cast<uint32>(indicies.size()));
		graphicsMgr.UploadResource(drawResource->indexBuffer);
	}

	drawResource->transformIndex = mesh->GetTransformIndex();
}

void revMeshRenderer::SetMaterials(const revArray<revMaterial*>& materials)
{
	for (uint32 i = 0; i < static_cast<uint32>(materials.size()); ++i) {
		SetMaterial(i, materials[i]);
	}
}

void revMeshRenderer::SetMaterial(uint32 index, revMaterial* material)
{
	if (static_cast<uint32>(materials.size()) <= index) {
		materials.resize(index + 1, nullptr);
	}
	materials[index] = material;
}

void revMeshRenderer::SetMaterialToAllSubMesh(revMaterial* material)
{
	materials.resize(1, material);
	for (uint32 i = 0; i < static_cast<uint32>(drawResources.size()); ++i) {
		drawResources[i]->materialIndex = 0;
	}
}

void revMeshRenderer::Update(const revMatrix44& world)
{
	uint32 transformsCount = static_cast<uint32>(transforms.size());
	for (uint32 i = 0; i < transformsCount; ++i) {
		transforms[i]->UpdateMatrix(world);
	}
}

void revMeshRenderer::Destroy()
{
	uint32 count = static_cast<uint32>(drawResources.size());
	for (uint32 i = 0; i < count; ++i) {
		if (drawResources[i] != nullptr) {
			if (drawResources[i]->vertexBuffer != nullptr) {
				drawResources[i]->vertexBuffer->Destroy();
				delete drawResources[i]->vertexBuffer;
			}
			if (drawResources[i]->indexBuffer != nullptr) {
				drawResources[i]->indexBuffer->Destroy();
				delete drawResources[i]->indexBuffer;
			}
			delete drawResources[i];
		}
	}
	drawResources.clear();
	count = static_cast<uint32>(transformConstantBuffers.size());
	for (uint32 i = 0; i < count; ++i) {
		if (transformConstantBuffers[i] != nullptr) transformConstantBuffers[i]->Destroy();
	}
	transformConstantBuffers.clear();
	// TODO:
	// revArray<revMaterial*> materials;
}