#ifndef __REVGRAPHICS_H__
#define __REVGRAPHICS_H__

#include "revSingleton.h"
#include "IDeviceContext.h" 
#include "revColor.h"
#include "revDrawCommand.h"

class Color;
class Window;
class IRenderer;


class revGraphics : public revSingleton<revGraphics>
{
public:
	void StartUp(Window* window, const GraphicsDesc& desc);
	void ShutDown();

	void ResizeWindow(int w, int h);

	void Draw();
private:
	GraphicsDesc desc;
	IRenderer* renderer;
    revDrawCommandStorage drawCommandStorage;
};

#endif
