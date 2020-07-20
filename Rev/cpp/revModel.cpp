#include "revModel.h"

void revModel::Destroy()
{
	uint32 transformCount = static_cast<uint32>(transforms.size());
	for (uint32 i = 0; i < transformCount; ++i) {
		if (transforms[i] != nullptr) {
			delete transforms[i];
			transforms[i] = nullptr;
		}
	}
	transforms.clear();

	uint32 meshCount = static_cast<uint32>(meshes.size());
	for (uint32 i = 0; i < meshCount; ++i) {
		if (meshes[i] != nullptr) {
			delete meshes[i];
			meshes[i] = nullptr;
		}
	}
	meshes.clear();



}
