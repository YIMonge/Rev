#ifndef __ITHREAD_H__
#define __ITHREAD_H__

#include "revTypedef.h" 
#include "IAsyncable.h" 

class IThread
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

	IThread() 
		: state(STATE::INITIALIZING)
	{}

	virtual ~IThread(){}

	virtual void Create(uint32 priority, uint32 stack_size, IAsyncable* func) = 0;
	virtual void Destroy() = 0;
	virtual void Suspend() = 0;
	virtual void Resume() = 0;
	virtual void Join() = 0;
	virtual void SetFunc(IAsyncable* func) = 0;

	STATE GetState() const
	{
		return state;
	}

protected:
	STATE state;
};

#endif
