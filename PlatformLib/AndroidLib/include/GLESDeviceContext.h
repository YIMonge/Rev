#ifndef __GLESDEVICECONTEXT_H__
#define __GLESDEVICECONTEXT_H__

#include "../../interface/IDeviceContext.h"

#ifdef _USE_GLES

class GLESDeviceContext : public IDeviceContext
{
public:
	GLESDeviceContext();
	virtual ~GLESDeviceContext();

	void Create();
	void Destroy();
};
#endif

#endif
