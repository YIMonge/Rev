#ifndef __REVAPP_H__
#define __REVAPP_H__

#include "revSingleton.h"

class Window;
class revScene;

// management application 
class revApp : public revSingleton<revApp>
{
public:
	revApp();
	
	void Run();

	bool isInitialized() const
	{
		return initialized;
	}

	virtual void StartUp(Window* window, revScene* scene);
	virtual void ShutDown();
private:
	float fixed_delta_time;
	float delta_time;
	float current_time;
	float last_time;
	bool initialized;
};



#endif