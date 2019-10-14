#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <windows.h>

class Window
{
public:
	Window();
	virtual ~Window();

	static LRESULT CALLBACK WndProc(HWND hwnd, unsigned int msg, WPARAM wparam, LPARAM lparam);

	virtual void Create(const char* name, int width, int height, int pos_x = 0, int pos_y = 0);
	virtual void Destroy();
	virtual void Disp();
	virtual void Hide();

	HWND GetHWnd(){ return hwnd; }
	unsigned int GetWidth();
	unsigned int GetHeight();

	bool IsDestroyed() const
	{
		return destroyed;
	}

protected:
	LRESULT WindowProc(HWND hwnd, unsigned int msg, WPARAM wparam, LPARAM lparam);

	virtual void OnCreate() {}
	virtual void OnDestroy() {
		PostQuitMessage(0); 
		destroyed = true;
	}
	virtual void OnInitDialog() {}
	virtual void OnClose() { destroyed = true; }
	virtual void OnPaint() {}
	virtual void OnTimer() {}
	virtual void OnSize(int cx, int cy);
	virtual void OnMouseMove(int cx, int cy){}
	virtual void OnKeyDown(WPARAM wParam){}
	virtual void OnCommand(WPARAM wParam){}
	
	bool DetachHWnd();
private:
	HWND hwnd;
	const char* name;
	unsigned int width;
	unsigned int height;
	bool destroyed;
};

inline unsigned int Window::GetWidth()
{
	return width;
}

inline unsigned int Window::GetHeight()
{
	return height;
}

#endif