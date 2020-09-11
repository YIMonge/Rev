#ifndef __REVBITSET_H__
#define __REVBITSET_H__

#include <bitset>
#include "revTypedef.h"

// TODO: allocator 
template<int size>
using revFixedBitset = std::bitset<size>;

class revBitset
{
public:
	revBitset(uint32 size)
	{
		this->size = size;
		uint32 index = size / sizeof(uint32);
		uint32 allocationSize = index == 0 ? 1 : index;
		buffer = new uint32[size / sizeof(uint32)];
		memset(buffer, 0, allocationSize * sizeof(uint32));
	}

	~revBitset()
	{
		delete[] buffer;
	}

	void On(uint32 no)
	{
		if (size <= no)
		{
			// TODO:assert
			return;
		}
		buffer[no / sizeof(uint32)] |= (1 << no % sizeof(uint32));
	}

	void Off(uint32 no)
	{
		if (size <= no)
		{
			// TODO:assert
			return;
		}
		buffer[no / sizeof(uint32)] &= ~(1 << no % sizeof(uint32));
	}

	bool Test(uint32 no) const 
	{
		if (size <= no)
		{
			// TODO:assert
			return false;
		}
		return (buffer[no / sizeof(uint32)] & (1 << no % sizeof(uint32))) != 0;
	}

private:
	uint32 size;
	uint32* buffer;
};

#endif
