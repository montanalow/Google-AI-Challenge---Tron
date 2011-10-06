/* time example */
#include <stdio.h>
#include <time.h>

#include <vector>
#include "map.h"
#include "bot.h"
#include <iostream>

int main ()
{
  zz::map map("./maps/empty-room.txt");
  int distance;


  distance = map.distance_between( zz::map::location( 1, 5), zz::map::location( 1, 5) );
  if( distance != 0 ){
    std::cerr << "The distance to 'here' was != 0" << std::endl;
  }
  distance = map.distance_between( map.player_one(), map.player_two() );
  if( distance != -1 ){
    std::cerr << "There should be no distance, since both players are a wall" << std::endl;
  }
  distance = map.distance_between( map.player_one(), zz::map::location(0,1) );
  if( distance != -1 ){
    std::cerr << "There was a distance to get on to a wall" << std::endl;
  }
  distance = map.distance_between( map.player_one(),zz::map::location( 1, 5) );
  if( distance != 4 ){
    std::cerr << "The distance was incorrect: expected 4, got " << distance << std::endl;
  }



//  std::cout << "distance from " << map.player_one() << " to " << map.player_two() << ": " << int (distance) << std::endl;
//  zz::a_star( distance, map, map.player_one(), map.player_two(), false);
//  std::cout << "distance from " << map.player_one() << " to " << map.player_two() << ": " << int (distance) << std::endl;
//  zz::a_star( distance, map, map.player_one(), map.player_two());
//  std::cout << "distance from " << map.player_one() << " to " << map.player_two() << ": " << int (distance) << std::endl;
//  zz::a_star( distance, map, map.player_two(), map.player_two());
//  std::cout << "distance from " << map.player_two() << " to " << map.player_two() << ": " << int (distance) << std::endl;
//  zz::map map("./maps/test.txt");
//  int player_one_area;
//  int player_two_area;
//  zz::voronoi_territory( player_one_area, player_two_area, map );
//  std::cerr << map;
//  std::cerr << map.player_one() << ": " << (int) player_one_area << " | " << map.player_two() << ": " << (int) player_two_area << std::endl;
//
//  int areas[4];
//  int max;
//  zz:max_available_areas( areas, max, map.player_one(), map );
//  for( int i = 0; i < 4; ++i ) {
//    std::cerr << ( int ) i << ":" << ( int ) areas[i] << ", ";
//  }
//  std::cerr << "]" << std::endl;

//  std::cout << zz::area_available_from( map, zz::map::location(1,2) ) << std::endl;
//  int sum = 0;
//  clock_t loop = clock();
//  for( int i = 0; i < map.width(); ++i ){
//    for( int j = 0; j < map.height(); ++j ){
//      sum += map.area_available_from( zz::map::location(i,j) );
//    }
//  }
//  loop = clock() - loop;
//  std::cout << "loop: " << loop << " : " << sum << std::endl;
//
//  sum = 0;
//  loop = clock();
//  for( int i = 0; i < map.width(); ++i ){
//    for( int j = 0; j < map.height(); ++j ){
//      sum += map.area_available_from( zz::map::location(i,j) );
//    }
//  }
//  loop = clock() - loop;
//  std::cout << "loop: " << loop << " : " << sum << std::endl;


//  int distance;
//  zz::distance_between( distance, map, map.player_one(), map.player_one());
//  std::cout << int (distance) << std::endl;
//  zz::distance_between( distance, map, map.player_one(), map.player_one().adjacent( zz::map::north ));
//  std::cout << int (distance) << std::endl;
//  zz::a_star( distance, map, map.player_one(), map.player_two());
//  std::cout << "distance from " << map.player_one() << " to " << map.player_two() << ": " << int (distance) << std::endl;
//  zz::a_star( distance, map, map.player_one(), zz::map::location(1,1));
//  std::cout << "distance from " << map.player_one() << " to " << zz::map::location(1,1) << ": " << int (distance) << std::endl;
//
//  zz::a_star( distance, map, map.player_one(), map.player_two());
//  std::cout << "distance from " << map.player_one() << " to " <<map.player_two() << ": " << int (distance) << std::endl;
//  zz::a_star( distance, map, map.player_one(), map.player_two());
//  std::cout << "distance from " << map.player_one() << " to " << map.player_two() << ": " << int (distance) << std::endl;
//  zz::a_star( distance, map, map.player_one(), map.player_two());
//  std::cout << "distance from " << map.player_one() << " to " << map.player_two() << ": " << int (distance) << std::endl;
//  zz::a_star( distance, map, map.player_two(), map.player_two());
//  std::cout << "distance from " << map.player_two() << " to " << map.player_two() << ": " << int (distance) << std::endl;


//  int next_scores_one[4];
//  int next_scores_two[4];
//  max_available_areas( next_scores_one, max_one, next_map.player_one(), next_map );
//  std::cerr << next_map.player_one() << " max_area: [";
//  for( int i = 0; i < 4; ++i ) {
//    std::cerr << ( int ) i << ":" << ( int ) next_scores_one[i] << ", ";
//  }
//  std::cerr << "] (" << ( int ) max_one << ")" << std::endl;
//  max_available_areas( next_scores_two, max_two, next_map.player_two(), next_map );
//  std::cerr << next_map.player_two() << " max_area: [";
//  for( int i = 0; i < 4; ++i ) {
//    std::cerr << ( int ) i << ":" << ( int ) next_scores_two[i] << ", ";
//  }
//  std::cerr << "] (" << ( int ) max_two << ")" << std::endl;
}

