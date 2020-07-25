#ifndef __REVMODEL_H__
#define __REVMODEL_H__

#include "revMesh.h"
#include "revMaterial.h"
#include "revResource.h"
#include "revTransform.h"

class revModel : public revResource
{
friend class revModelLoader;
public:
	revModel() {}
	virtual ~revModel(){}

	void Destroy();

	const revArray<revMesh*>& GetMeshes() const { return meshes; }
	const revArray<revTransform*>& GetTransforms() const { return transforms; }

#ifdef _DEBUG
	void AddMesh(revMesh* mesh) { meshes.push_back(mesh); }
	void AddTransform(revTransform* transform) { transforms.push_back(transform); }
#endif
	class ImportSetting
	{
	public:
		ImportSetting() :
			readOnly(true),
			usageForCollision(false)
		{}
		~ImportSetting(){}


		bool IsReadOnly() const { return readOnly; }
		bool IsUsageForCollision() const { return usageForCollision; }
		
		
		SERIALIZE_FUNC()
		{
			archive(REV_NVP(readOnly),
				REV_NVP(usageForCollision)
				);
		}
	private:
		bool readOnly;				// if readonly checked, mdl doesn't have each array, has only vertexbufferdata.
		bool usageForCollision;		// if the flag checked, mdl has vertex array for collision
	};

	const ImportSetting& GetImportSetting() const { return importSetting; }

	SERIALIZE_FUNC()
	{
		archive(
			SERIALIZE_BASE_CLASS(revResource),
			REV_NVP(importSetting)
		);
	}

private:
	ImportSetting importSetting;
	revArray<revTransform*> transforms;
	revArray<revMesh*> meshes;
	revArray<revMaterial*> materials;
};

#endif

