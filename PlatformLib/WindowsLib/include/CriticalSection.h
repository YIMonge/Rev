#ifndef __CRITICALSECTION_H__
#define __CRITICALSECTION_H__

#include <Windows.h>
namespace Thread
{
	class CriticalSection
	{
	public:
		CriticalSection()
		{
			InitializeCriticalSection(&cs);
		}

		~CriticalSection()
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
}

#endif
