#ifndef __REVARRAY_INL__
#define __REVARRAY_INL__


template<typename T>
revArray<T>::revArray() 
: size(0)
, count(10)
{
	// default capacity is 10
	array = new T[count];
}

template<typename T>
revArray<T>::revArray(uint32 capacity)
: size(0)
, count(capacity)
{
	array = new T[count];
}

template<typename T>
revArray<T>::~revArray()
{
	delete[] array;
	array = nullptr;
}

template<typename T>
void revArray<T>::Add(const T& obj)
{
	GrowIfNeeded();
	array[size++] = obj; 
}

template<typename T>
void revArray<T>::Remove(uint32 index)
{
	// TODO : memmove
}

template<typename T>
void revArray<T>::Remove(const T& obj)
{
	// TODO : implement Find 
}

template<typename T>
T& revArray<T>::operator [] (int32 i)
{
	return array[i];
}

template<typename T>
void revArray<T>::Resize(uint32 capacity)
{
	size = capacity;
	T* new_array = new T[capacity];
	if(array != nullptr){
		memcpy(new_array, array, count * sizeof(T));
		delete[] array;
	}
	count = capacity;
	array = new_array;	
}

template<typename T>
uint32 revArray<T>::Count() const
{
	return size;
}

template<typename T>
void revArray<T>::GrowIfNeeded()
{
	if(size + 1 < count)
	{
		return;
	}

	T* new_array = new T[count * 2];
	
	if(array != nullptr){
		memcpy(new_array, array, sizeof(T) * count);
		delete[] array;
	}
	count *= 2;
	array = new_array;
}

#endif