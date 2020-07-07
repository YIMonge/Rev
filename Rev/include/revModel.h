#ifndef __REVMODEL_H__
#define __REVMODEL_H__

#include "revMesh.h"
#include "revMaterial.h"
#include "revResource.h"

class revModel : public revResource
{
friend class revModelLoader;
public:
	revModel() {}
	virtual ~revModel(){}

	const revArray<revMesh>& GetMeshes() const { return meshes; }

#ifdef _DEBUG
	void AddMesh(const revMesh& mesh) { meshes.push_back(mesh); }
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
		archive(REV_NVP(metaData),
			REV_NVP(importSetting)
		);
	}

private:
	DefaultMetaData metaData;
	ImportSetting importSetting;

	revArray<revMesh> meshes;
	revArray<uint32> materials;
};

#endif
