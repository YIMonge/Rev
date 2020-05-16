#include "revGameObjectManager.h"
#include "Log.h"

const char* DEFAULT_GROUP_NAME[] =
{
	"SYSTEM",
	"PLAYER",
	"CAMERA",
	"GAMEOBJECT",
	"EFFECT",
	"BACKGROUND",
};

void revGameObjectManager::StartUp()
{
	revString metaPath = RESOURCE_PATH;
	metaPath += "gameobjectmanager.meta";

	if (File::isExist(metaPath.c_str())) {
		revSerializer::Deserialize(metaPath.c_str(), metaData);
	}
	else {
		for (uint32 i = 0; i < SIZE_OF_ARRAY(DEFAULT_GROUP_NAME); ++i) {
			metaData.groupInfos[i].name = DEFAULT_GROUP_NAME[i];
		}
		revSerializer::Serialize(metaPath.c_str(), metaData);
	}
}

void revGameObjectManager::ShutDown()
{

}


void revGameObjectManager::Update()
{

}

void revGameObjectManager::Add(revGameObject* gameObjcet, const char* name)
{
	for (uint32 i = 0; i < MAX_GROUP_NUM; ++i)
	{
		if (metaData.groupInfos[i].name == name) {
			Add(gameObjcet, i);
			return;
		}
	}
	NATIVE_LOGE("could not find gameobject group. FILE:%s, LINE:%d", __FILE__, __LINE__);	
}

void revGameObjectManager::Add(revGameObject* gameObject, uint32 group)
{
	managedObjects[group].push_back(gameObject);
	if (gameObject->IsUseUpdate()) asyncTasks[group].push_back(AsyncUpdate(gameObject));
	if (gameObject->IsUseLateUpdate()) asyncLateTasks[group].push_back(AsyncLateUpdate(gameObject));
}
