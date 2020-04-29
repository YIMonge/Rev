#ifndef __THREAD_H__
#define __THREAD_H__

#include "revThread.h"
#include <Windows.h>

class Thread : revThread
{
public:
	Thread();
	virtual ~Thread();		

	virtual void Create(uint32 priority, uint32 stack_size, revAsyncable* func);
	virtual void Destroy();
	virtual void Suspend();
	virtual void Resume();
	virtual void Join();
	virtual void SetFunc(revAsyncable* func);

	protected:
		static uint32 __stdcall ThreadFunc(void* thread);
		void ThreadFuncImpl();

		HANDLE handle;
		uint32 stack_size;
		uint32 id;
		uint32 priority;
		revAsyncable* func;
};


#endif