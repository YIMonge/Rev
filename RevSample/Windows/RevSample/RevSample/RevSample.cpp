// RevSample.cpp : アプリケーションのエントリ ポイントを定義します。
#include "RevSample.h"
#include "revMemory.h"
#include "revGraphics.h"
#include "revWindow.h"

#include "revBitset.h"
#include "revThread.h"
#include "revThreadPool.h"
#include "revArray.h"


#include <stdio.h>

revThread thread;
class Async : public IAsyncable
{
public:
	virtual bool AsyncFunc()
	{
		r += 0.01f;
		if (r > 1.0f) {
			r = 0.0f;
		}

		revGraphics::Get().Draw();
		Sleep(1);
		return false;
	}

	float r;
};




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	revMemory::Get().StartUp(ToMegaByte(256));
	GraphicsDesc desc;
	desc.bufferFormat = GraphicsDesc::BUFFERFORMAT::R8B8G8A8;
	desc.bufferType = GraphicsDesc::BUFFERTYPE::DOUBLE_BUFFER;
	desc.useFullscreen = false;

	Window* window = new revWindow();
	window->Create("MainWindow", 1280, 780);
	revGraphics::Get().StartUp(window, desc);

	revThreadPool::WorkerInfo workerInfo(0, ToKiloByte(1024), 1024);
	revThreadPool threadPool(workerInfo, 8);

	thread.Create(0, ToKiloByte(1024), new Async());
		
	float r = 0.0f;
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (window->IsDestroyed() == false){
		if (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE) == FALSE)
		{
			SleepEx(1, TRUE);
		}
		else
		{
			if (GetMessage(&msg, nullptr, 0, 0) == FALSE) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	revGraphics::Get().ShutDown();
	revMemory::Get().ShutDown();
	return 0;
}