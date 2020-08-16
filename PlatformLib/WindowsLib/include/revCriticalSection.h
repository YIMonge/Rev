#ifndef __REVCRITICALSECTION_H__
#define __REVCRITICALSECTION_H__

#include <Windows.h>
class revCriticalSection
{
public:
	revCriticalSection()
	{
		InitializeCriticalSection(&cs);
	}

	~revCriticalSection()
	{
		DeleteCriticalSection(&cs);
	}

	void Lock() const
	{
		if (!TryEnterCriticalSection(&cs)){
			EnterCriticalSection(&cs);
		}
	}

	void Unlock() const
	{
		LeaveCriticalSection(&cs);
	}

	mutable CRITICAL_SECTION cs;
};

#endif
