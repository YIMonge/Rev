#include "revGameObjectManager.h"

const uint32 RESERVE_GAMEOBJECT_SIZE = 128;

void revGameObjectManager::StartUp()
{
	for (uint32 i = 0; i < GAMEOBJECT_PRIORITY_MAX_NUM; ++i)
	{
		gameObjects[i].reserve(RESERVE_GAMEOBJECT_SIZE);
	}

}

void revGameObjectManager::Update()
{
	// TODO: MultiThread 
	for (uint32 i = 0; i < GAMEOBJECT_PRIORITY_MAX_NUM; ++i)
	{
		uint32 count =static_cast<uint32>(gameObjects[i].size());
		for (uint32 j = 0; j < count; ++j) {
			gameObjects[i][j]->Update();
		}
	}

	for (uint32 i = 0; i < GAMEOBJECT_PRIORITY_MAX_NUM; ++i)
	{
		uint32 count = static_cast<uint32>(gameObjects[i].size());
		for (uint32 j = 0; j < count; ++j) {
			gameObjects[i][j]->SyncUpdate();
		}
	}

}

void revGameObjectManager::ShutDown()
{

}
