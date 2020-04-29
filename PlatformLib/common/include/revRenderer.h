#ifndef __REVRENDERER_H__
#define __REVRENDERER_H__

class Window;

#include "revTypedef.h"
#include "revDevice.h"

class revColor;
enum class BLEND_FUNC : uint8;
enum class ALPHA_TEST : uint8;

class revRenderer
{
public:
	revRenderer(){}
	virtual ~revRenderer(){};

	virtual void StartUp(Window* window, const GraphicsDesc& desc) = 0;
	virtual void ShutDown() = 0;

	virtual void Render() = 0;
};

enum class BLEND_FUNC : uint8
{
    NONE,
    ALPHABLEND,
    ADDITIVE,

};

enum class ALPHA_TEST : uint8
{
	NONE,
	LESS,
	EQUAL,
	LEQUAL,
	GREATER,
	NOTEQUAL,
	GEQUAL,
	ALWAYS,
};

class IRenderState
{
public:
	virtual ~IRenderState(){};

	virtual void SetBlendFunc(BLEND_FUNC func) = 0;
	virtual void SetAlphaTest(ALPHA_TEST func, float value) = 0;

protected:
	BLEND_FUNC 	blend_func;
	ALPHA_TEST 	alpha_test;
	float 		alpha_value;
};


#endif