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
class revMeshRenderer;

class revGraphics : public revSingleton<revGraphics>
{
public:
	void StartUp(Window* window, const GraphicsDesc& desc);
	void ShutDown();

	void ResizeWindow(int w, int h);

	void BeginLoad();	// should call the function before load graphics resource
	void EndLoad();		// upload graphics resource to vram 

	void Draw();	

	revGraphicsBuffer* CreateVertexBuffer();
	revGraphicsBuffer* CreateIndexBuffer();
	revGraphicsBuffer* CreateConstantBuffer();

	revMeshRenderer* CreateMeshRenderer();
	revDevice* GetDevice() { return renderer->GetDevice(); }

private:
	GraphicsDesc desc;
	revRenderer* renderer;
    revDrawCommandStorage drawCommandStorage;
};

#endif
