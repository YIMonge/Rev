#ifndef __REVARRAY_INL__
#define __REVARRAY_INL__


template<typename T>
revArray<T>::revArray() 
: size(0)
{
	// default size is 10
	array = new T[10];
}

template<typename T>
revArray<T>::revArray(uint32 capacity)
: size(0)
{
	array = new T[capacity];
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
void revArray<T>::SetCapacity(uint32 capacity)
{
	T* new_array = new T[capacity];
	if(array != nullptr){
		memcpy(new_array, array, sizeof(array));
		delete[] array;
	}
	array = new_array;	
}

template<typename T>
uint32 revArray<T>::GetCapacity() const
{
	return sizeof(array) / sizeof(T);
}

template<typename T>
void revArray<T>::GrowIfNeeded()
{
	uint32 capacity = GetCapacity();
	if(size + 1 < capacity)
	{
		return;
	}

	T* new_array = new T[capacity * 2];
	
	if(array != nullptr){
		memcpy(new_array, array, sizeof(array));
		delete[] array;
	}
	array = new_array;
}

#endif