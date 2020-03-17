#ifndef __REVASYNCABLE_H__
#define __REVASYNCABLE_H__

class revAsyncable
{
public:
	// if you want to finish thread, return true
	virtual bool AsyncFunc() = 0;
};



#endif