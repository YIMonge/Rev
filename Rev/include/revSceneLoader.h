#ifndef __REVSCENELOADER_H__
#define __REVSCENELOADER_H__

#include "revScene.h"
#include "revResourceLoader.h"

class revSceneLoader : public revResourceLoader<revScene>
{
public:
	revSceneLoader();
	virtual ~revSceneLoader();

	virtual bool LoadFromFile(const char* path, revScene* scene);

#ifdef _DEBUG
	void WriteScene(const char* path, revScene* scene);
#endif

protected:
	virtual bool OnLoadFailed(const char* resourcePath, revScene* scene);

};

#endif

