#ifndef __REVDICTIONARY_H__
#define __REVDICTIONARY_H__

#include "revTypedef.h"
#include "revIterator.h"
#include <unordered_map>

template<typename KEY, typename VALUE>
using revDictionary = std::unordered_map<KEY, VALUE>;

#endif

