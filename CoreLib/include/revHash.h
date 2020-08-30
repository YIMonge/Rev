#ifndef __REVHASH_H__
#define __REVHASH_H__


#include <type_traits>

class revHash
{
public:
	static uint64 GetHash(const char* path)
	{
		uint64 key = 0;
		uint32 length = static_cast<uint32>(strlen(path)) + 1;
		for (uint32 i = 0; i < length; ++i) {
			key = 31 * key + path[i];
		}
		return key;
	}
};


#endif
