#ifndef __ILOCKER_H__
#define __ILOCKER_H__

class ILocker
{
public:
	virtual void Lock() = 0;
	virtual void Unlock() = 0;
	
};

#endif