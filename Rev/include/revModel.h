#ifndef __REVMODEL_H__
#define __REVMODEL_H__

#include "revMesh.h"
#include "revMaterial.h"

class revModel
{
public:
	revModel() {}
	~revModel(){}

#ifdef _DEBUG
	void AddMesh(const revMesh& mesh) { meshes.push_back(mesh); }
#endif

private:
	revArray<revMesh> meshes;
	revArray<revMaterial> materials;
};

#endif
