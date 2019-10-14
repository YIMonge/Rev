#ifndef __REVBITSET_H__
#define __REVBITSET_H__

template<typename T, int size>
class revBitset
{
public:
	revBitset();
	~revBitset();

	void On(T flag);
	void Off(T flag);
	void Reset();
	bool Test(T flag) const;
	bool Any() const;
public:	
	char buffer[size/CHAR_BIT + 1];
};

#include "revBitset.inl"


#endif
