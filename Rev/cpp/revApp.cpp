#include "revApp.h"
#include "revMemory.h"
#include "revGraphics.h"

revApp::revApp()
: fixed_delta_time(0.0f)
, delta_time(0.0f)
, current_time(0.0f)
, last_time(0.0f)
, initialized(false)
{
}

/* virtual */ void revApp::StartUp(Window* window)
{
	revMemory::Get().StartUp(ToMegaByte(128));
	GraphicsDesc desc;
	revGraphics::Get().StartUp(window, desc);

	initialized = true;
}

/* virtual */ void revApp::ShutDown()
{
	revMemory::Get().ShutDown();
	revGraphics::Get().ShutDown();
}

void revApp::Run()
{
	if(!initialized) return;
		revGraphics::Get().Clear(true, false, revColor::blue);

	fixed_delta_time = 0.0f;
	delta_time = 0.0f;
	current_time = 0.0f;
	last_time = 0.0f;
	//while (true){
		revGraphics::Get().RenderBegin();

		revGraphics::Get().RenderEnd();
	//}
}
