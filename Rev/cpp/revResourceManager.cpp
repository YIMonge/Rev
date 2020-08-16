#include "revResourceManager.h"


template<>
revModel* revResourceManager::Load<revModel>(const char* path)
{
	return Load<revModel, revModelLoader>(path);
}

template<>
revTexture* revResourceManager::Load<revTexture>(const char* path)
{
#ifdef _USE_DIRECTX12
	return Load<DX12Texture, revTextureLoader>(path);
#elif defined(_USE_VULKAN)
	return Load<VulkanTexture, revTextureLoader>(path);
#endif
}

template<>
revMaterial* revResourceManager::Load<revMaterial>(const char* path)
{
	return Load<revMaterial, revMaterialLoader>(path);
}

template<>
revShader* revResourceManager::Load<revShader>(const char* path)
{
#ifdef _USE_DIRECTX12
	return Load<DX12Shader, revShaderLoader>(path);
#elif defined(_USE_VULKAN)
	return Load<VulkanShader, revShaderLoader>(path);
#endif
}