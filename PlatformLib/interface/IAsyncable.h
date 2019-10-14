#ifndef __IASYNCABLE_H__
#define __IASYNCABLE_H__

class IAsyncable
{
public:
	// if you want to finish thread, return true
	virtual bool AsyncFunc() = 0;
};



#endif