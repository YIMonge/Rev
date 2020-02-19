#ifndef __REVARRAY_H__
#define __REVARRAY_H__

#include "revTypedef.h"
#include "revIterator.h"
#include <vector>

template<typename T>
using revArray = std::vector<T>;

/*
template<typename T>
class revArray
{
public:
	revArray();
	revArray(uint32 capacity);
	~revArray();

	void Add(const T& obj);
	void Remove(uint32 index);
	void Remove(const T& obj);
	T& operator [] (int32 i);
	const T& operator [] (int32 i) const;

	void Resize(uint32 capacity);
	uint32 Count() const;
	void Fill(const T& val);
	void Fill(const T& val, uint32 size);

	ITERATOR_DEF(T, std::forward_iterator_tag);
	iterator begin()
	{
		return iterator(&array[0]);
	}
	iterator end()
	{
    	return iterator(&array[size - 1]);
	}	

private:
	void GrowIfNeeded();

	T* array;
	uint32 size;
	uint32 count;
};

#include "revArray.inl"
*/
#endif