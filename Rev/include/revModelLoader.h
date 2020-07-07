#ifndef __REVMODELLOADER_H__
#define __REVMODELLOADER_H__

#include "revModel.h"

class revModelLoader
{
public:
	revModelLoader();
	virtual ~revModelLoader();

	bool LoadFromFile(const char* path, revModel* model);

private:
	bool LoadMetaFile(const char* metaPath, revModel* model);
#ifdef _WINDOWS
	void writeMDL(const char* path, const revModel& model);
#endif

};

#endif
