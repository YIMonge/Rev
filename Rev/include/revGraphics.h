#ifndef __REVGRAPHICS_H__
#define __REVGRAPHICS_H__

#include "revSingleton.h"
#include "IDeviceContext.h" 
#include "revColor.h"

class Color;
class Window;
class IRenderer;

class revGraphics : public revSingleton<revGraphics>
{
public:
	void StartUp(Window* window, const GraphicsDesc& desc);
	void ShutDown();

	void ResizeWindow(int w, int h);

	void RenderBegin();
	void RenderEnd();

	void Clear(bool clear_color, bool clear_depth, const revColor& fill_color);
	void SwapBuffers();


private:
	GraphicsDesc desc;
	IRenderer* renderer;
};

#endif
