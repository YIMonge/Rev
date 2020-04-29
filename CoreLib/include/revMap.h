#ifndef __REVMAP_H__
#define __REVMAP_H__

#include "revTypedef.h"
#include <map>
#include <unordered_map>

template<typename KEY, typename VALUE>
using revMap = std::map<KEY, VALUE>;

template<typename KEY, typename VALUE>
using revUnorderedMap = std::unordered_map<KEY, VALUE>;


#endif