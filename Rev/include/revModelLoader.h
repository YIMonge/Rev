#ifndef __REVMODELLOADER_H__
#define __REVMODELLOADER_H__

#include "revModel.h"
#include "revResourceLoader.h"

class revModelLoader : public revResourceLoader<revModel>
{
public:
	revModelLoader();
	virtual ~revModelLoader();

	virtual bool LoadFromFile(const char* path, revModel* model);

protected:
	virtual bool OnLoadFailed(const char* resourcePath, revModel* model);

private:
#ifdef _WINDOWS
	bool LoadFBXFromFile(const char* path, revModel* model);
	void WriteMDL(const char* path, const revModel& model);	
#endif

};

#endif
