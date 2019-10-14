#ifndef __REVITERATOR_H__
#define __REVITERATOR_H__

#include <iterator>

#define ITERATOR_DEF(T, C)  \
class iterator : public std::iterator<C, T> \
{ \
public: \
	iterator() \
	: obj(nullptr) \
	{} \
\
	iterator(T* begin) \
	: obj(begin) \
	{} \
\
    T& operator *() const { \
        return *obj; \
    } \
\
    iterator& operator ++(){ \
        ++obj; \
        return *this; \
    } \
\
    iterator operator ++(int){ \
        T* p = obj; \
        ++(*this); \
        return iterator(p); \
	} \
\
    iterator& operator --(){ \
        --obj; \
        return *this; \
    } \
\
    const bool operator !=(const iterator& it) const { \
        return (obj != &*it); \
    } \
\
    const bool operator ==(const iterator& it) const { \
        return (obj == &*it); \
    } \
\
private: \
	T* obj; \
\
}; \

#endif