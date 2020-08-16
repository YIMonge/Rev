#ifndef __REVSCENE_H__
#define __REVSCENE_H__

#include "revString.h"
#include "revResource.h"
#include "revArray.h"
#include "revResourceManager.h"

class revScene : public revResource
{
public:
	revScene()
	{
	}

	virtual ~revScene()
	{
	}

	virtual void Load();
	virtual void Destroy();



protected:
	revArray<revString> shaderResources;
	revArray<revString> textureResources;
	revArray<revString> materialResources;
	revArray<revString> modelResources;


	revArray<revResource*> resources;
};


#endif
