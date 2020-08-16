#ifndef __REVSEMAPHORE_H__
#define __REVSEMAPHORE_H__

#include <thread>
#include <windows.h>
#include "revString.h"

class revSemaphore
{
public:
	revSemaphore(uint32 count, uint32 maxCount, const char* name)
	{
		this->name = name;
		handle = CreateSemaphore(NULL, count, maxCount, (LPCWSTR)name);
	}

	void Wait()
	{
		WaitForSingleObject(handle, INFINITE);
	}

	void Release(uint32 releaseCount = 1)
	{
		ReleaseSemaphore(handle, releaseCount, nullptr);
	}


	// TODO:
private:
	HANDLE handle;
	revString name;
};



#endif
