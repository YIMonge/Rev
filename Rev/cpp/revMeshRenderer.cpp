#include "revMeshRenderer.h"
#include "revGraphics.h"
#include "Log.h"

revMeshRenderer::revMeshRenderer()
{
}

revMeshRenderer::~revMeshRenderer()
{
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
		NATIVE_LOGE("faile create vertex buffer. file:%s , line:%s", __FILE__, __LINE__);
	}
	vertexBuffers[index]->Create(mesh.GetVertexData());
}

