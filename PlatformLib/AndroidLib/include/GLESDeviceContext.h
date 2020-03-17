#ifndef __GLESDEVICECONTEXT_H__
#define __GLESDEVICECONTEXT_H__

#include "revDeviceContext.h"

#ifdef _USE_GLES

class GLESDeviceContext : public revDeviceContext
{
public:
	GLESDeviceContext();
	virtual ~GLESDeviceContext();

	void Create();
	void Destroy();
};
#endif

#endif
