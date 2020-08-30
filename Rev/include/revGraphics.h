#ifndef __REVGRAPHICS_H__
#define __REVGRAPHICS_H__

#include "revSingleton.h"
#include "revDevice.h"
#include "revColor.h"
#include "revDrawCommand.h"
#include "revRenderer.h"
#include "revDictionary.h"
#include "revGraphicsResource.h"
#include "revQueue.h"

class Color;
class Window;
class revGraphicsBuffer;
class revMeshRenderer;
class revUploadableResource;

/// <summary>
/// Graphics System 
/// can create graphics object by Graphics Library. 
/// </summary>
class revGraphics : public revSingleton<revGraphics>
{
public:
	void StartUp(Window* window, const GraphicsDesc& desc);
	void ShutDown();

	void ResizeWindow(int w, int h);

	// TODO : delete
	void BeginLoad();	// should call the function before load graphics resource
	void EndLoad();		// upload graphics resource to vram 

	void Update();

	void Draw();	

	void Test();

	revGraphicsBuffer* CreateVertexBuffer();
	revGraphicsBuffer* CreateIndexBuffer();
	revGraphicsBuffer* CreateConstantBuffer();
	revMeshRenderer* CreateMeshRenderer();
	revDevice* GetDevice() { return renderer->GetDevice(); }

private:
	GraphicsDesc desc;
	revRenderer* renderer;

    revDrawCommandStorage drawCommandStorage;

	revDictionary<uint64, revGraphicsResource*> graphicsResources;

	/// <summary>
	/// resources are waiting for upload
	/// the queue accessed from multi thread
	/// </summary>
	revQueue<revUploadableResource*> readyForUploadResources;
	/// <summary>
	/// resources are upploading 
	/// </summary>
	revQueue<revUploadableResource*> uploadingResources;
};

#endif
