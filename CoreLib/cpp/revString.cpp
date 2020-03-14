/*
#include "../include/revString.h"
#include <string.h>

revString::revString(const char* source)
{
	buffer = new char[strlen(source) + 1];
	strcpy(buffer, source);
}

revString::revString(const revString& source)
{
	buffer = new char[source.Count() + 1];
	strcpy(buffer, source.GetCString());
}

revString::~revString()
{
	if(buffer != nullptr){
		delete[] buffer;
	}
}

unsigned int revString::Count() const
{
	return strlen(buffer);
}

const char* revString::GetCString() const
{
	return buffer;
}

void revString::Append(const revString& str)
{
	Append(str.GetCString());
}

void revString::Append(const char* str)
{
	unsigned int count = Count();
	char* newBuffer = new char[count + strlen(str) + 1];
	memcpy(newBuffer, GetCString(), count + 1);
	strcat(newBuffer, str);
	delete[] buffer;
	buffer = newBuffer;
}

revString 	revString::operator +  (const revString& rhs)
{
	revString ret = *this;
	ret.Append(rhs);
	return ret;
}

revString& 	revString::operator += (const revString& rhs)
{
	this->Append(rhs);
	return *this;
}
*/
