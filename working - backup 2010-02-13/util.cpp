#include "util.h"

bool zz::map_value_comparator(const std::pair<zz::map::direction, int>& lhs, const std::pair<zz::map::direction, int>& rhs) {
  return lhs.second < rhs.second;
}

bool zz::location_map_value_comparator(const std::pair<zz::map::location, int>& lhs, const std::pair<zz::map::location, int>& rhs) {
  return lhs.second < rhs.second;
}
