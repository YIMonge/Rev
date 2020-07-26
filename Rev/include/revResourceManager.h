#ifndef __REVRESOURCEMANAGER_H__
#define __REVRESOURCEMANAGER_H__

#include "revSingleton.h"
#include "revDictionary.h"
#include "revResourceLoader.h"
#include "revModelLoader.h"
#include "revMaterialLoader.h"

class revResourceManager : public revSingleton<revResourceManager>
{
public:
	revResourceManager()
	{
	}

	virtual ~revResourceManager()
	{
	}

	template<typename T> 
	T* Load(const char* path)
	{
		return nullptr;
	}

private:
	template<typename T, typename LOADER>
	T* Load(const char* path)
	{
		T* resource = new T();
		LOADER loader;
		if (!loader.LoadFromFile(path, resource)) {

			return nullptr;
		}
		if (resources.find(resource->GetUUID()) == resources.end()) {
			NATIVE_LOGE("ResouceManager Hash confrict. file1:%s file2:%s", resource->GetFilePath(), resources[resource->GetUUID()]->GetFilePath());
			return resource;
		}
		resources.emplace(resource->GetUUID(), resource);
		return resource;

	}

	revDictionary<uint64, revResource*> resources;
};

template<>
revModel* revResourceManager::Load<revModel>(const char* path)
{
	return Load<revModel, revModelLoader>(path);
}

template<>
revMaterial* revResourceManager::Load<revMaterial>(const char* path)
{
	return Load<revMaterial, revMaterialLoader>(path);
}

#endif