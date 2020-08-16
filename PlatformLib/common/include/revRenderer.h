#ifndef __REVRENDERER_H__
#define __REVRENDERER_H__

class Window;

#include "revTypedef.h"
#include "revDevice.h"
#include "revArray.h"
#include "revGraphicsCommandList.h"

class revRenderer
{
public:
	revRenderer(){}
	virtual ~revRenderer(){};

	virtual void StartUp(Window* window, const GraphicsDesc& desc) = 0;
	virtual void ShutDown() = 0;

	virtual void OpenGlobalCommandList() = 0;
	virtual void CloseGlobalCommandList() = 0;

	virtual void ExecuteCommand(revArray<revGraphicsCommandList>& lists) = 0;
	virtual void ExecuteCommand(revGraphicsCommandList& list) = 0;

	virtual void Render() = 0;

	virtual revDevice* GetDevice() = 0;
};


#endif