#ifndef __REVBITSET_H__
#define __REVBITSET_H__

#include <bitset>

// TODO: allocator 
template<int size>
using revBitset = std::bitset<size>;

#endif
