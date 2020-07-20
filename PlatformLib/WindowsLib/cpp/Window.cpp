#include "../include/Window.h"
#include "../../../Rev/include/revGraphics.h"

// ウィンドウに関連付けるプロパティ名
static LPCSTR PROP_NAME = "WinMainProp";

Window::Window()
	: destroyed(false)
{
}

Window::~Window()
{
}

void Window::Create(const char* name, int width, int height, int pos_x/* = 0 */, int pos_y/* = 0*/)
{
	WNDCLASSEX wc = {};

	this->width = width;
	this->height = height;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = name;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = 0;

	RegisterClassEx(&wc);

	hwnd = ::CreateWindow(
		wc.lpszClassName, 
		(LPCSTR)"Game", 
		WS_OVERLAPPEDWINDOW, 
		pos_x, 
		pos_y, 
		width, 
		height, 
		NULL,
		NULL,
		wc.hInstance, 
		NULL
	);

	SetProp(hwnd, PROP_NAME, (HANDLE)this);
}

void Window::Destroy()
{
	DetachHWnd();
}

void Window::Disp()
{
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
}

void Window::Hide()
{

}

LRESULT Window::WndProc(HWND hwnd, unsigned int msg, WPARAM wparam, LPARAM lparam)
{
	Window* target = (Window*)GetProp(hwnd, PROP_NAME);

	if (target == NULL){
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return target->WindowProc(hwnd, msg, wparam, lparam);
}

void Window::OnSize(int cx, int cy)
{
	//revGraphics::Get().ResizeWindow(cx, cy);
}

LRESULT Window::WindowProc(HWND hwnd, unsigned int msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg){
	case WM_MOVE:			OnMouseMove(LOWORD(lparam), HIWORD(lparam));	break;
	case WM_PAINT:			OnPaint();		break;
	case WM_LBUTTONDOWN:	break;
	case WM_CLOSE:			OnClose();					return 0;
	case WM_KEYDOWN:		OnKeyDown(wparam);			break;
	case WM_SIZE:			OnSize(LOWORD(lparam), HIWORD(lparam)); break;
	case WM_DESTROY:		OnDestroy();				break;
	case WM_COMMAND:		OnCommand(LOWORD(wparam));	break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

bool Window::DetachHWnd()
{
	if (hwnd == NULL){
		return false;
	}

	if (GetProp(hwnd, PROP_NAME)){
		RemoveProp(hwnd, PROP_NAME);
	}

	return true;
}