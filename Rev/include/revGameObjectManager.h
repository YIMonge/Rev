#ifndef __REVGAMEOBJECTMANAGER_H__
#define __REVGAMEOBJECTMANAGER_H__

#include "revSingleton.h"
#include "revGameObject.h"
#include "revString.h"
#include "revSerializer.h"
#include "revThreadPool.h"


struct GameObjectGroupInfo
{
	revString name;
	f32 timeScale;
	bool isActive;
	SERIALIZE_FUNC() {
		archive(REV_NVP(name));
	}
};

class revGameObjectManager : public revSingleton<revGameObjectManager>
{
public:
	void StartUp();
	void ShutDown();

	void Add(revGameObject* gameObjcet, const char* name);
	void Add(revGameObject* gameObject, uint32 group);

	void Update();


	static const uint32 MAX_GROUP_NUM = 128;
	struct MetaData
	{
		GameObjectGroupInfo groupInfos[MAX_GROUP_NUM];
	};

	class AsyncUpdate : public revAsyncable
	{
	public:
		AsyncUpdate(revGameObject* obj) :
			owner(obj)
		{
		}
		bool AsyncFunc() {
			owner->Update();
			return false;
		}
	private:
		revGameObject* owner;
	};

	class AsyncLateUpdate : public revAsyncable
	{
	public:
		AsyncLateUpdate(revGameObject* obj) :
			owner(obj)
		{
		}
		bool AsyncFunc() {
			owner->LateUpdate();
			return false;
		}
	private:
		revGameObject* owner;
	};

private:
	revArray<revGameObject*> managedObjects[MAX_GROUP_NUM];
	revArray<AsyncUpdate> asyncTasks[MAX_GROUP_NUM];
	revArray<AsyncLateUpdate> asyncLateTasks[MAX_GROUP_NUM];

	MetaData metaData;
};


SERIALIZE_FUNC_NON_INTRUSIVE(revGameObjectManager::MetaData, metaData)
{
	archive(REV_NVP(metaData.groupInfos));
}

#endif
