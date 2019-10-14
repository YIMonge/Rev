#ifndef __ITEXTURE_H__
#define __ITEXTURE_H__

namespace Graphics
{
	class revTextureHandle;

	class ITexture
	{
	public:
		virtual revTextureHandle GetHandle();
	};
}

#endif