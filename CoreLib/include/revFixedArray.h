#ifndef __REVFIXEDARRAY_H__
#define __REVFIXEDARRAY_H__

#include <array>
#include "revTypedef.h"

template<typename T, int32 NUM>
using revFixedArray = std::array<T, NUM>;

#endif