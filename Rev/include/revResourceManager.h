#ifndef __REVRESOURCEMANAGER_H__
#define __REVRESOURCEMANAGER_H__

#include "revSingleton.h"
#include "revDictionary.h"
#include "revResourceLoader.h"
#include "revModelLoader.h"
#include "revMaterialLoader.h"
#include "revShaderLoader.h"
#include "revTextureLoader.h"
#ifdef _USE_DIRECTX12
#include "DX12Shader.h"
#include "DX12Texture.h"
#elif defined(_USE_VULKAN)
#include "VulkanShader.h"
#include "VulkanTexture.h"
#endif


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
		// already loaded ?
		uint64 hash = revHash::GetHash(path);
		if (resources.find(hash) != resources.end()) {
			return reinterpret_cast<T*>(resources[hash]);
		}

		T* resource = new T();
		LOADER loader;
		if (!loader.LoadFromFile(path, resource)) {

			return nullptr;
		}
		// TODO: hash confrict
		if (resources.find(resource->GetUUID()) != resources.end()) {
		}
		resources.emplace(resource->GetUUID(), resource);
		return resource;

	}

	revDictionary<uint64, revResource*> resources;
};

template<>
revModel* revResourceManager::Load<revModel>(const char* path);
template<>
revTexture* revResourceManager::Load<revTexture>(const char* path);
template<>
revMaterial* revResourceManager::Load<revMaterial>(const char* path);
template<>
revShader* revResourceManager::Load<revShader>(const char* path);
#endif