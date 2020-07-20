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
	bool LoadFBXFromFile(const char* path, revModel* model);
	void WriteMDL(const char* path, const revModel& model);	
#endif

};

#endif
