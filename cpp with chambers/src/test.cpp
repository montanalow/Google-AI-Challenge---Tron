/* time example */
#include <stdio.h>
#include <time.h>

#include "map.h"
#include "bot.h"
#include <iostream>

std::ostream & operator<<( std::ostream& output, const zz::map& map ) {
  for( zz::map::coordinate y = 0; y < map.height( ); ++y ) {
    for( zz::map::coordinate x = 0; x < map.width( ); ++x ) {
      zz::map::location location( x, y );
      if( map.player_one( ) == location ) {
        output << "A";
      } else if( map.player_two( ) == location ) {
        output << "B";
      } else if( map.is_wall( x, y ) ) {
        output << "#";
      } else if( map.is_hallway( zz::map::location( x, y ) ) ) {
        output << map.is_hallway( zz::map::location( x, y ) ) % 10;
      } else {
        output << " ";
      }
    }
    output << std::endl;
  }
  return output;
}

std::ostream & operator<<( std::ostream& output, const zz::map::location& location ) {
  output << "(" << ( int ) location.x( ) << "," << ( int ) location.y( ) << ")";
  return output;
}

void test_hallways() {
  zz::map map( "./maps/test-area.txt" );
  std::cout << map << std::endl;
  map.set_is_wall(zz::map::location(6,4), false);
  std::cout << map << std::endl;
  map.set_is_wall(zz::map::location(6,4), true);
  std::cout << map << std::endl;
  map.set_is_wall(zz::map::location(8,4), false);
  std::cout << map << std::endl;
}

void test_territory() {
  zz::map map( "./maps/triangle.txt" );
  std::cout << map << map.territory() << std::endl;
  std::cout << map.minimax(5) << std::endl;
  zz::map::location player_one = map.player_one();
  zz::map::location player_two = map.player_two();
  
  map.set_player_one( player_one.adjacent( 0 ) );
  std::cout << map << map.territory() << std::endl;

  map = zz::map( "./maps/triangle.txt" );
  map.set_player_one( player_one.adjacent( 1 ) );
  std::cout << map << map.territory() << std::endl;

  map = zz::map( "./maps/triangle.txt" );
  map.set_player_one( player_one.adjacent( 0 ) );
  map.set_player_two( player_two.adjacent( 0 ) );
  std::cout << map << map.territory() << std::endl;

  map = zz::map( "./maps/triangle.txt" );
  map.set_player_one( player_one.adjacent( 0 ) );
  map.set_player_two( player_two.adjacent( 3 ) );
  std::cout << map << map.territory() << std::endl;

  map = zz::map( "./maps/triangle.txt" );
  map.set_player_one( player_one.adjacent( 1 ) );
  map.set_player_two( player_two.adjacent( 0 ) );
  std::cout << map << map.territory() << std::endl;

  map = zz::map( "./maps/triangle.txt" );
  map.set_player_one( player_one.adjacent( 1 ) );
  map.set_player_two( player_two.adjacent( 3 ) );
  std::cout << map << map.territory() << std::endl;
}

int test_minimax(int depth) {
  zz::map map( "./maps/test1.txt" );
  std::cout << map << std::endl;
  clock_t timer = clock();
  map.minimax(depth);
  // build a list of the best moves
  int count_moves = 0;
  int moves[4];
  for( int i = 0; i < 4; ++i){
    if( map.mins()[i] == map.max() ){
      moves[count_moves++] = i;
    }
  }
  timer = clock() - timer;
  
  fprintf( stderr, " minimax(%d): [", depth );
  for( int i = 0; i < 4; ++i ) {
    fprintf( stderr, "%d:%d", i, map.mins()[i] );
    if( i !=  3) fprintf( stderr, ", " );
  }
  fprintf( stderr, "] (%d) %-.4fs\n", map.max(), (float) timer / CLOCKS_PER_SEC );

}

void test_bot( const char* filename ) {
  zz::bot bot;
  FILE* f = fopen( filename, "r" );
  if( f ) {
    bot.set_map( f );
    bot.move();
    fclose( f );
  } else {
    fprintf( stderr, "could not open map: %s\n", filename );
    fflush( stderr );
  }
}
//void paint_line() {
//  zz::map map( "./maps/huge-room.txt" );
//  map.set_player_one( zz::map::location(15,7) );
//  zz::map::location center( map.width() / 2, map.height() / 2 );
////  float slope = (float) (center.y() - map.player_one().y()) / (center.x() - map.player_one().x());
////  float x = center.x();
////  float y = center.y();
////  while( x < map.width() && x > 0 && y < map.width() && y > 0 ) {
////    zz::map::location location( x, y );
////    map.set_is_wall( location, true );
////    if( map.player_one().x() < center.x() ) {
////      x -= 0.1;
////    } else {
////      x += 0.1;
////    }
////    y = center.y() + (x - center.x()) * slope;
////  }
//  int left_x = std::min( map.player_one().x(), center.x() );
//  int right_x = std::max( map.player_one().x(), center.x() );
//  int top_y = std::min( map.player_one().y(), center.y() );
//  int bottom_y = std::max( map.player_one().y(), center.y() );
//  float slope = (float) (top_y - bottom_y) / (left_x - right_x);
//  for( int x = left_x; x <= right_x; ++x ){
//    for( int y = top_y; y < bottom_y; ++y ) {
//      if( y == (int) slope * x ){
//        map.set_is_wall( zz::map::location(x,y) );
//      }
//    }
//  }
//  std::cout << map << std::endl;
//}

int main( ) {
//  paint_line();
  
//  zz::map map( "./maps/empty-room.txt" );
//  int distance;
//
//  distance = map.distance_between( zz::map::location( 1, 5 ), zz::map::location( 1, 5 ) );
//  if( distance != 0 ) {
//    std::cerr << "The distance to 'here' was != 0" << std::endl;
//  }
//  distance = map.distance_between( map.player_one( ), map.player_two( ) );
//  if( distance != -1 ) {
//    std::cerr << "There should be no distance, since both players are a wall" << std::endl;
//  }
//  distance = map.distance_between( map.player_one( ), zz::map::location( 0, 1 ) );
//  if( distance != -1 ) {
//    std::cerr << "There was a distance to get on to a wall" << std::endl;
//  }
//  distance = map.distance_between( map.player_one( ), zz::map::location( 1, 5 ) );
//  if( distance != 4 ) {
//    std::cerr << "The distance was incorrect: expected 4, got " << distance << std::endl;
//  }
//
//  if(! map.locations_are_connected( map.player_one(), map.player_two() ) ){
//    std::cerr << "Player one and two should be connected on an empty map!" << std::endl;
//  }

//  test_hallways();
//  test_territory();
  for( int i = 0; i < 30; ++i ){
    test_minimax(i);
  }
//  for( int i = 0; i < 100; ++i ){
//    test_bot( "./maps/test1.txt");
//    fprintf( stderr, "-----------------------------------------\n");
//  }
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
  return 0;
}

