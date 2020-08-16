#include "revApp.h"
#include "revMemory.h"
#include "revGraphics.h"
#include "revSceneManager.h"
#include "Window.h"


revApp::revApp()
: fixed_delta_time(0.0f)
, delta_time(0.0f)
, current_time(0.0f)
, last_time(0.0f)
, initialized(false)
{
}

void revApp::StartUp(Window* window, revScene* scene)
{
	revMemory::Get().StartUp(ToMegaByte(128));
	GraphicsDesc desc = {};
	revGraphics& graphics = revGraphics::Get();
	graphics.StartUp(window, desc);

	// TODO: splito to another thread
	graphics.BeginLoad();
	revSceneManager::Get().StartUp(scene);
	graphics.EndLoad();

	initialized = true;
}

void revApp::ShutDown()
{
	revMemory::Get().ShutDown();
	revGraphics::Get().ShutDown();
	revSceneManager::Get().ShutDown();
}

void revApp::Run()
{
	fixed_delta_time = 0.0f;
	delta_time = 0.0f;
	current_time = 0.0f;
	last_time = 0.0f;
	revGraphics::Get().Draw();
}
