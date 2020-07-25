#ifndef __REVHASH_H__
#define __REVHASH_H__


#include <type_traits>

template<typename T>
using revHash = std::hash<T>;

#endif
