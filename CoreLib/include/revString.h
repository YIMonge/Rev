#ifndef __REVSTRING_H__
#define __REVSTRING_H__

#include "revTypedef.h"
#include <string>

//TODO: set allocator 
using revString = std::string;

//#include "revMemory.h"

/*
class revString
{
//	MEM_DEFINE(revString, Memory::VATIAVLE_ARRAY);
public:
	revString(const char* source);
	revString(const revString& source);

	~revString();

	unsigned int Count() const;
	const char* GetCString() const;
	
	void Append(const revString& str);
	void Append(const char* str);

	revString 	operator +  (const revString& rhs);
	revString& 	operator += (const revString& rhs);
private:	
	char* buffer;
};
*/

#endif