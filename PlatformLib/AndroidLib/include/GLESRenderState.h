#ifndef __GLESRENDERSTATE_H__
#define __GLESRENDERSTATE_H__

#include "../../interface/IRenderer.h"
#ifdef _USE_GLES


class GLESRenderState : public IRenderState
{
public:
	GLESRenderState();
	GLESRenderState(const GLESRenderState& source);
	virtual ~GLESRenderState();
	virtual void SetBlendFunc(BLEND_FUNC func);
	virtual void SetAlphaTest(ALPHA_TEST func, float value);
};

#endif
#endif