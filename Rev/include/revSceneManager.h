#ifndef __RECSCENEMANAGER_H__
#define __REVSCENEMANAGER_H__

#include "revSingleton.h"
#include "revStack.h"
#include "revScene.h"


class revSceneManager : public revSingleton<revSceneManager>
{
public:
	revSceneManager()
	{}

	virtual ~revSceneManager()
	{}

	bool StartUp(revScene* entry);
	void ShutDown();

private:
	revStack<revScene*> scenes;
};


#endif
