#ifndef __REVGRAPHICS_H__
#define __REVGRAPHICS_H__

#include "revSingleton.h"
#include "revDevice.h"
#include "revColor.h"
#include "revDrawCommand.h"
#include "revRenderer.h"

class Color;
class Window;
class revGraphicsBuffer;

class revGraphics : public revSingleton<revGraphics>
{
public:
	void StartUp(Window* window, const GraphicsDesc& desc);
	void ShutDown();

	void ResizeWindow(int w, int h);

	void Draw();	

	revGraphicsBuffer* CreateVertexBuffer();
	revGraphicsBuffer* CreateIndexBuffer();

	revDevice* GetDevice() { return renderer->GetDevice(); }

private:
	GraphicsDesc desc;
	revRenderer* renderer;
    revDrawCommandStorage drawCommandStorage;
};

#endif
