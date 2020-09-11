#ifndef __REVTEXTURELOADER_H__
#define __REVTEXTURELOADER_H__

#include "revModel.h"
#include "revResourceLoader.h"

class revTextureLoader : public revResourceLoader<revTexture>
{
public:
	revTextureLoader();
	virtual ~revTextureLoader();

	virtual bool LoadFromFile(const char* path, revTexture* texture);

protected:
	virtual void OnLoadComplete(revTexture* texture);
};

#endif
