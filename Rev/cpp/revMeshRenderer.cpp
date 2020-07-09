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
	transforms = revArray<revTransform>(model->GetTransforms());
	uint32 count = static_cast<uint32>(transforms.size());
	constantBuffers.resize(count);

	revMatrix44 matrixies[2];

	for (uint32 i = 0; i < count; ++i) {
		matrixies[0] = transforms[i].GetLocalMatrix();
		matrixies[1] = transforms[i].GetWorldMatrix();

		constantBuffers[i] = revGraphics::Get().CreateConstantBuffer();
		constantBuffers[i]->Create(matrixies, sizeof(matrixies), 1, revGraphicsBuffer::USAGE::DYNAMIC);
	}
	SetMeshes(model->GetMeshes());
}

void revMeshRenderer::SetMeshes(const revArray<revMesh>& meshes)
{
	for (uint32 i = 0; i < static_cast<uint32>(meshes.size()); ++i) {
		SetMesh(i, meshes[i]);
	}
}

void revMeshRenderer::SetMesh(uint32 index, const revMesh& mesh)
{
	if (static_cast<uint32>(vertexBuffers.size()) <= index) {
		vertexBuffers.resize(index + 1, nullptr);
	}
	vertexBuffers[index] = revGraphics::Get().CreateVertexBuffer();
	if (vertexBuffers[index] == nullptr) {
		NATIVE_LOGE("failed create vertex buffer. file:%s , line:%s", __FILE__, __LINE__);
	}
	const auto& vertices = mesh.GetVertexData();
	const uint32 sizeOfBytes = mesh.GetSizeOfBytes();
	vertexBuffers[index]->Create(vertices.data(), sizeOfBytes, static_cast<uint32>(vertices.size()) / (sizeOfBytes / sizeof(f32)));

	if (mesh.GetIndexArray().size() > 0) {
		if (static_cast<uint32>(indexBuffers.size()) <= index) {
			indexBuffers.resize(index + 1, nullptr);
		}

		indexBuffers[index] = revGraphics::Get().CreateIndexBuffer();
		if (indexBuffers[index] == nullptr) {
			NATIVE_LOGE("failed create index buffer. file:%s , line:%s", __FILE__, __LINE__);
		}
		const auto& indicies = mesh.GetIndexArray();
		indexBuffers[index]->Create(indicies.data(), sizeof(revIndex3), static_cast<uint32>(indicies.size()));
	}
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
	materials.resize(vertexBuffers.size(), material);
}

void revMeshRenderer::Update(const revMatrix44& world)
{
	uint32 transformsCount = static_cast<uint32>(transforms.size());
	for (uint32 i = 0; i < transformsCount; ++i) {
		transforms[i].UpdateMatrix(world);
	}
}