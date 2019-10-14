#include <string.h>

template<typename T, int size>
revBitset<T, size>::revBitset()
{
	memset(buffer, 0, sizeof(buffer));
}

template<typename T, int size>
revBitset<T, size>::~revBitset()
{
}

template<typename T, int size>
void revBitset<T, size>::On(T flag)
{
	buffer[flag/CHAR_BIT] |= (1 << (flag%CHAR_BIT));
}

template<typename T, int size>
void revBitset<T, size>::Off(T flag)
{
	buffer[flag/CHAR_BIT] &= ~(1 << (flag%CHAR_BIT));
}

template<typename T, int size>
void revBitset<T, size>::Reset()
{
	memset(buffer, 0, sizeof(buffer));
}

template<typename T, int size>
bool revBitset<T, size>::Test(T flag) const
{
	return (buffer[flag/CHAR_BIT] & (1 << (flag%CHAR_BIT))) != 0;
}

template<typename T, int size>
bool revBitset<T, size>::Any() const
{
	for(int i = 0; i < size/CHAR_BIT + 1; ++i)
	{
		if(buffer[i] != 0)
		{
			return true;
		}
	}
	return false;
}