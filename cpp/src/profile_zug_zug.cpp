#include "map.h"

int main() {
  zz::map map1("../maps/profile-minimax.txt");
  map1.minimax(6);

  zz::map map2("../maps/profile-solomax.txt");
  map2.solomax(12);

  return 0;
}
