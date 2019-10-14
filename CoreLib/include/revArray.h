#ifndef __REVARRAY_H__
#define __REVARRAY_H__

#include "revTypedef.h"
#include "revIterator.h"

template<typename T>
class revArray
{
public:
	revArray();
	revArray(uint32 capacity);

	void Add(const T& obj);
	void Remove(uint32 index);
	void Remove(const T& obj);
	T& operator [] (int32 i);

	void SetCapacity(uint32 capacity);
	uint32 GetCapacity() const;

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
};

#include "revArray.inl"

#endif