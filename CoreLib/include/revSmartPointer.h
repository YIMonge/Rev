#ifndef __REVSMARTPOINTER_H__
#define __REVSMARTPOINTER_H__


template<typename T> 
class revSmartPtr
{
public:
	explicit revSmartPtr(T* _ptr=nullptr);
	revSmartPtr(const revSmartPtr<T>& _ptr);
	virtual ~revSmartPtr();

	const revSmartPtr<T>& operator=(T* _ptr);
	const revSmartPtr<T>& operator=(const revSmartPtr<T>& _ptr);

	const T& operator*();
	T** operator&();
	T* const operator->();

private:
	T* ptr;
	int* counter;

	void release();
};

#include "revSmartPointer.inl"

#endif
