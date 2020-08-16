#ifndef __REVGAMEOBJCETMANAGER_H__
#define __REVGAMEOBJECTMANAGER_H__

#include "revSingleton.h"
#include "revGameObject.h"
#include "revArray.h"
#include "Log.h"

class revGameObjectManager : public revSingleton<revGameObjectManager>
{
public:
	revGameObjectManager(){}
	virtual ~revGameObjectManager(){}

	void StartUp();	
	// TODO: MultiThread update
	void Update();
	void ShutDown();

	void AddGameObject(revGameObject* obj, uint32 priority)
	{
		if (priority >= GAMEOBJECT_PRIORITY_MAX_NUM) {
			NATIVE_LOGE("Failed Add GameObject. priority : %d", priority);
			return;
		}

		gameObjects[priority].push_back(obj);
	}

	static const uint32 GAMEOBJECT_PRIORITY_MAX_NUM = 64;
private:
	revArray<revGameObject*> gameObjects[GAMEOBJECT_PRIORITY_MAX_NUM];
};


#endif
