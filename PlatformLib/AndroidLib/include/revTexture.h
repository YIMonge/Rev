#ifndef __REVTEXTURE_H__
#define __REVTEXTURE_H__

#include "../../interface/ITexture.h"

class revString;

namespace Graphics
{
	class revTextureHandle
	{
	friend class revTexture;
	public:
		revTextureHandle();
		revTextureHandle(unsigned int value);
		unsigned int Get();

	private:
		unsigned int handle;
	};
	
	inline unsigned int revTextureHandle::Get()
	{
		return handle;
	}	


	class revTexture : public ITexture
	{
	public:
		revTexture(int width, int height);
		revTexture(const revString& path);
	private:		
		unsigned int width;
		unsigned int height;
		revTextureHandle handle;
	};

}

#endif