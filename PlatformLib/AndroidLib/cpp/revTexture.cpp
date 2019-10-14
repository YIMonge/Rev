#include "../include/revTexture.h"
#include <EGL/egl.h>
#include <GLES/gl.h>


namespace Graphics
{
	revTextureHandle::revTextureHandle() :
	handle(0)
	{		
	}

	revTextureHandle::revTextureHandle(unsigned int value) :
	handle(value)
	{
	}

	revTexture::revTexture(int width, int height)
	{
		this->width = width;
		this->height = height;

		glGenTextures(1, &(handle.handle));
	}

	revTexture::revTexture(const revString& path)
	{
		// TODO : open and read texture file;

	}

}