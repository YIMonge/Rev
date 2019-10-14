#include "revSmartPointer.h"
#include "revMemory.h"

template<typename T>
revSmartPtr<T>::revSmartPtr(T* _ptr)
: ptr(_ptr)
, counter(nullptr)
{	
	if(_ptr){
		counter = new int(1);
	}
}

template<typename T>
revSmartPtr<T>::revSmartPtr(const revSmartPtr<T>& _smartptr)
{
	ptr = _smartptr.ptr;
	counter = _smartptr.counter;
	if(counter){
		++(*counter);
	}
}

template<typename T>
revSmartPtr<T>::~revSmartPtr()
{
	release();
}

template<typename T> 
void revSmartPtr<T>::release()
{
	if (counter)
	{
		(*counter)--;
		if ((*counter) > 0)
		{
			return;
		}

		SAFE_DELETE(counter);
		SAFE_DELETE(ptr);	
	}
}

template<typename T> 
const T& revSmartPtr<T>::operator*()
{
	return *ptr;
}

template<typename T>
inline T ** revSmartPtr<T>::operator&()
{
	return &ptr;
}

template<typename T> 
T* const revSmartPtr<T>::operator->()
{
	return ptr;
}

template<typename T> 
const revSmartPtr<T>& revSmartPtr<T>::operator=(T* _ptr)
{
	release();

	ptr = _ptr;
	counter = 0;
	if (ptr)
	{
		counter = new int(1);
	}

	return (*this);
} 

template<typename T> 
const revSmartPtr<T>& revSmartPtr<T>::operator=(const revSmartPtr<T>& _ptr)
{
	release();

	ptr = _ptr.pointerptr;
	counter = _ptr.counter;
	if (counter){
		++(*counter);
	}
	return (*this);
}
