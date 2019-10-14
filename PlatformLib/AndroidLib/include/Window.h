#ifndef __WINDOW_H__
#define __WINDOW_H__

struct ANativeWindow;

class Window
{
public:
	Window(ANativeWindow* window);
	ANativeWindow* GetHandle()
	{
		return window;
	}

private:
	ANativeWindow* window;
};


#endif