#include "revSceneManager.h"
#include "Log.h"

bool revSceneManager::StartUp(revScene* entry)
{
	if (entry == nullptr) {
		NATIVE_LOGE("Entry Scene is null");
		return false;
	}


	entry->Load();
	scenes.push(entry);

	return true;
}

void revSceneManager::ShutDown()
{

}