#ifndef __REVWINDOW_H__
#define __REVWINDOW_H__

#include "Window.h"

class revWindow : public Window
{
public:
	virtual void Create(const char* name, int width, int height, int pos_x = 0, int pos_y = 0)
	{
		Window::Create(name, width, height, pos_x, pos_y);
	}

	virtual void Destroy()
	{
		Window::Destroy();
	}

	virtual void Disp()
	{
		Window::Disp();
	}

	virtual void Hide()
	{
		Window::Hide();
	}

	virtual void OnPaint()
	{
	}
	
};


#endif