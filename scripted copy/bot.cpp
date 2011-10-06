/* 
 * File:   Bot.cpp
 * Author: montana
 * 
 * Created on February 8, 2010, 2:49 PM
 */
#include <algorithm>

#include "bot.h"
#include "map.h"

zz::bot::bot( )
: _map() {
  std::cerr << "Initialized new bot" << std::endl;
}

zz::bot::~bot( ) {
}

void zz::bot::move( ) {
  zz::map::direction direction = calculate_move( );
  _map.load_from_file( stdin );
  output_move( direction );
}

zz::map::direction zz::bot::calculate_move( ) {
  zz::map::location location = _map.player_one();
  int adjacent_availabilities[4] = {0,0,0,0};
  for( int i = 0; i < 4; ++i){
    adjacent_availabilities[i] = _map.area_available_from( location.adjacent( i ) );
  }
  int max_area = *std::max_element(adjacent_availabilities, adjacent_availabilities+4);
  for( int i = 0; i < 4; ++i){
    if( adjacent_availabilities[i] == max_area ) return i;
  }
  return zz::map::south;
}

void zz::bot::output_move( zz::map::direction direction ) {
  // The four possible moves the contest engine recognizes are:
  //   * 1 -- North. Negative Y direction.
  //   * 2 -- East. Positive X direction.
  //   * 3 -- South. Positive X direction.
  //   * 4 -- West. Negative X direction.
  int move = 1;
  switch (direction) {
    case zz::map::north:
        move = 1;
      break;
    case zz::map::east:
        move = 2;
      break;
    case zz::map::south:
        move = 3;
      break;
    case zz::map::west:
        move = 4;
      break;
  }
  std::cerr << "Outputting move: " << (int) move << std::endl;
  std::cout << (int) move << std::endl;
}
