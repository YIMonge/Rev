#include "../include/GLESRenderState.h"

#ifdef _USE_GLES
#include <GLES/gl.h>
#include <math.h>

void GLESRenderState::SetBlendFunc(BLEND_FUNC func)
{
	if(blend_func == func){
		return;
	}

	if(blend_func == BLEND_FUNC::NONE && func != BLEND_FUNC::NONE){
		glEnable(GL_BLEND);
	}

	switch(func){
		case BLEND_FUNC::NONE:
			glDisable(GL_BLEND);
			break;

		case BLEND_FUNC::ALPHABLEND:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;

		case BLEND_FUNC::ADDITIVE:
			glBlendFunc(GL_ONE, GL_ONE);
			break;

		default:
			break;		
	}
}

void GLESRenderState::SetAlphaTest(ALPHA_TEST func, float value)
{
	if(alpha_test == func && fabs(value - alpha_value) < 0.001f){
		return;
	}

	switch(func){
		case ALPHA_TEST::NONE:
			glDisable(GL_ALPHA_TEST);
			break;

		case ALPHA_TEST::LESS:
			glAlphaFunc(GL_LESS, value);
			break;

		default:
			break;
	}
}

#endif