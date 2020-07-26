#ifndef __REVMATERIALLOADER_H__
#define __REVMATERIALLOADER_H__

#include "revMaterial.h"
#include "revResourceLoader.h"
#include "revGraphics.h"

class revMaterialLoader : revResourceLoader<revMaterial>
{
public:
	revMaterialLoader();
	virtual ~revMaterialLoader();

	virtual bool LoadFromFile(const char* path, revMaterial* material);

#ifdef _WINDOWS
	void WriteMaterial(const char* path, const revMaterial& material);
#endif
};

#endif

