// RevSample.cpp : アプリケーションのエントリ ポイントを定義します。
#include "RevSample.h"
#include "revApp.h"
#include "revWindow.h"
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Window* window = new revWindow();
	window->Create("MainWindow", 1280, 780);	
	revApp::Get().StartUp(window);
	window->Disp();
		
	float r = 0.0f;
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (window->IsDestroyed() == false){
		if (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE) == FALSE)
		{
			revApp::Get().Run();
			SleepEx(1, TRUE);
		}
		else
		{
			if (GetMessage(&msg, nullptr, 0, 0) == FALSE) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	revApp::Get().ShutDown();
	return 0;
}