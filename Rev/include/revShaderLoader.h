#ifndef __REVSHADERLOADER_H__
#define __REVSHADERLOADER_H__

#include "revShader.h"
#include "revResourceLoader.h"
#include "revGraphics.h"

class revShaderLoader : public revResourceLoader<revShader>
{
public:

	virtual bool LoadFromFile(const char* path, revShader* resource)
	{
		if (resource == nullptr) return false;

		resourcePath = RESOURCE_PATH;
		resourcePath += path;
		resource->SetFilePath(path);

		SHADER_TYPE type = SHADER_TYPE::VERTX;
		if (strstr(path, "_frag") != nullptr) {
			type = SHADER_TYPE::FRAGMENT;
		}
		resource->LoadFromFile(revGraphics::Get().GetDevice(), path, type);
	
		return true;
	}
};

#endif
