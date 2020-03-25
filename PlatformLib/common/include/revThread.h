#ifndef __REVTHREAD_H__
#define __REVTHREAD_H__

#include "revTypedef.h" 
#include "revAsyncable.h"

class revThread
{
public:
	enum class STATE : uint8
	{
		INITIALIZING = 0,
		RUNNING,
		SUSPEND,
		WAIT_FOR_DESTROY,
		DESTROYED,
	};

	revThread()
		: state(STATE::INITIALIZING)
	{}

	virtual ~revThread(){}

	virtual void Create(uint32 priority, uint32 stack_size, revAsyncable* func) = 0;
	virtual void Destroy() = 0;
	virtual void Suspend() = 0;
	virtual void Resume() = 0;
	virtual void Join() = 0;
	virtual void SetFunc(revAsyncable* func) = 0;

	STATE GetState() const
	{
		return state;
	}

protected:
	STATE state;
};

#endif
