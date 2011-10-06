/*
 * File:   util.h
 * Author: montana
 *
 * Created on February 8, 2010, 2:49 PM
 */

#ifndef _ZZ_UTIL
#define _ZZ_UTIL

#include "map.h"

namespace zz {
  bool map_value_comparator(const std::pair<zz::map::direction, int>& lhs, const std::pair<zz::map::direction, int>& rhs);
  bool location_map_value_comparator(const std::pair<zz::map::location, int>& lhs, const std::pair<zz::map::location, int>& rhs);
}

#endif // _ZZ_UTIL