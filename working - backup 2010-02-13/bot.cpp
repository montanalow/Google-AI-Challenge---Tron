/* 
 * File:   Bot.cpp
 * Author: montana
 * 
 * Created on February 8, 2010, 2:49 PM
 */
#include "bot.h"
#include "benchmark.h"

#include <stdio.h>
#include <time.h>
#include <algorithm>

zz::bot::bot( )
: _script( ), _scripted_move( _script.end( ) ), _map( ), _apocalyptic( "./maps/apocalyptic.txt" ), _big_room( "./maps/big_room.txt" ), _center( "./maps/center.txt" ), _divider( "./maps/divider.txt" ), _joust( "./maps/joust.txt" ), _keyhole( "./maps/keyhole.txt" ), _medium_room( "./maps/medium_room.txt" ), _playground( "./maps/playground.txt" ), _quadrant( "./maps/quadrant.txt" ), _ring( "./maps/ring.txt" ), _toronto( "./maps/toronto.txt" ), _trix( "./maps/trix.txt" ), _u( "./maps/u.txt" ) {
}

zz::bot::~bot( ) {
}

void zz::bot::set_map( FILE * file ) {
  _map.load_from_file( file );
}

void zz::bot::move( ) {
#ifdef BENCHMARK
  clock_t timer = clock();
#endif

  if( running_script( ) ) {
    std::cerr << "executing scripted move" << std::endl;
    output_move( ( *_scripted_move++ ) );
    return;
  }

  // get the minimax
  int max;
  int next_score[4];
  int maxes = 0;
  int iterations = 4;
  clock_t move_clock = clock();
  do {
    minimax( next_score, max, _map, iterations );
    ++iterations;
    for( int i = 0; i < 4; ++i ) { if( next_score[i] == max ) ++maxes; }
  } while( maxes > 1 && max < 512 && (float) (clock() - move_clock) / CLOCKS_PER_SEC < 0.1f );

  std::map<zz::map::direction, int> directions_minimax_scores;
  for( int i = 0; i < 4; ++i ) {
    if( next_score[i] == max ) {
      directions_minimax_scores[i] = next_score[i];
    }
  }

  // orderly progressions of moves seem to do better than random or charging
  // move_first( directions_minimax_scores );
  // move_random( directions_minimax_scores );
  // move_closer( directions_minimax_scores );
  // move_center( directions_minimax_scores );
  // move_to_wall( directions_minimax_scores );
  if( _map.locations_are_connected( _map.player_one(), _map.player_two() ) ) {
    move_voronoi( directions_minimax_scores );
  } else {
    move_first( directions_minimax_scores );
  }

#ifdef DEBUG
  std::cerr << _map.player_one( ) << " vs " << _map.player_two( ) << " : possible moves:  [";
  for( std::map<zz::map::direction, int>::iterator it = directions_minimax_scores.begin( ); it != directions_minimax_scores.end( ); ++it ) {
    std::cerr << ( int ) ( *it ).first << ":" << ( int ) ( *it ).second << ", ";
  }
  std::cerr << "]" << std::endl;
#endif
#ifdef BENCHMARK
  zz::benchmark::bot_move += clock() - timer;
  std::cerr << "bot_minimax: " << (float) zz::benchmark::bot_minimax / CLOCKS_PER_SEC << std::endl;
  std::cerr << "bot_move: " << (float) zz::benchmark::bot_move / CLOCKS_PER_SEC << std::endl;
#endif
}

bool zz::bot::running_script( ) {
  if( _scripted_move != _script.end( ) ) {
    std::cerr << "script is running:" << std::endl;
    return true;
  }
  if( _map == _apocalyptic ) {
    if( _map.player_one() == zz::map::location(20,2) ) {
      int script[16] = {zz::map::north,zz::map::west,zz::map::west,zz::map::west,zz::map::west,zz::map::west,zz::map::west,zz::map::west,zz::map::south,zz::map::south,zz::map::south,zz::map::south,zz::map::south,zz::map::south,zz::map::south,zz::map::south};
      set_script( script,16 );
    } else {
      int script[16] = {zz::map::south,zz::map::east,zz::map::east,zz::map::east,zz::map::east,zz::map::east,zz::map::east,zz::map::east,zz::map::north,zz::map::north,zz::map::north,zz::map::north,zz::map::north,zz::map::north,zz::map::north,zz::map::north};
      set_script( script, 16 );
    }
    return true;
  } else if( _map == _big_room ) {
    //    if( _map.player_one() == zz::map::location(3,3) ) {
    //      int script[6] = {1,1,1,1,2,2};
    //      set_script( script,6 );
    //    } else {
    //      int script[6] = {3,3,3,3,0,0};
    //      set_script( script, 6 );
    //    }
    //    return true;
  } else if( _map == _center ) {
    //    if( _map.player_one() == zz::map::location(3,3) ) {
    //      int script[6] = {1,1,1,1,2,2};
    //      set_script( script,6 );
    //    } else {
    //      int script[6] = {3,3,3,3,0,0};
    //      set_script( script, 6 );
    //    }
    //    return true;
  } else if( _map == _divider ) {
    // default plays a perfect game
    //    if( _map.player_one() == zz::map::location(3,3) ) {
    //      int script[6] = {1,1,1,1,2,2};
    //      set_script( script,6 );
    //    } else {
    //      int script[6] = {3,3,3,3,0,0};
    //      set_script( script, 6 );
    //    }
    //    return true;
  } else if( _map == _joust ) {
    if( _map.player_one( ) == zz::map::location( 1, 1 ) ) {
      int script[20] = { zz::map::south, zz::map::south, zz::map::south, zz::map::south, zz::map::south, zz::map::south, zz::map::south, zz::map::south, zz::map::south, zz::map::south, zz::map::east, zz::map::east, zz::map::east, zz::map::east, zz::map::east, zz::map::east, zz::map::east, zz::map::east, zz::map::east, zz::map::east };
      set_script( script, 20 );
    } else {
      int script[20] = { zz::map::north, zz::map::north, zz::map::north, zz::map::north, zz::map::north, zz::map::north, zz::map::north, zz::map::north, zz::map::north, zz::map::north, zz::map::east, zz::map::east, zz::map::east, zz::map::east, zz::map::east, zz::map::east, zz::map::east, zz::map::east, zz::map::east, zz::map::east };
      set_script( script, 20 );
    }
    return true;
  } else if( _map == _keyhole ) {
    if( _map.player_one( ) == zz::map::location( 18, 6 ) ) {
      int script[13] = { zz::map::east, zz::map::south, zz::map::west, zz::map::south, zz::map::south, zz::map::south, zz::map::south, zz::map::west, zz::map::west, zz::map::west, zz::map::west, zz::map::west, zz::map::west };
      set_script( script, 13 );
    } else {
      int script[13] = { zz::map::west, zz::map::north, zz::map::east, zz::map::north, zz::map::north, zz::map::north, zz::map::north, zz::map::east, zz::map::east, zz::map::east, zz::map::east, zz::map::east, zz::map::east };
      set_script( script, 13 );
    }
    return true;
  } else if( _map == _medium_room ) {
    //    if( _map.player_one() == zz::map::location(3,3) ) {
    //      int script[6] = {1,1,1,1,2,2};
    //      set_script( script,6 );
    //    } else {
    //      int script[6] = {3,3,3,3,0,0};
    //      set_script( script, 6 );
    //    }
    //    return true;
  } else if( _map == _playground ) {
    //    if( _map.player_one() == zz::map::location(3,3) ) {
    //      int script[6] = {1,1,1,1,2,2};
    //      set_script( script,6 );
    //    } else {
    //      int script[6] = {3,3,3,3,0,0};
    //      set_script( script, 6 );
    //    }
    //    return true;
  } else if( _map == _quadrant ) {
    if( _map.player_one( ) == zz::map::location( 3, 3 ) ) {
      // int script[11] = { zz::map::north, zz::map::west, zz::map::south, zz::map::west, zz::map::north, zz::map::north, zz::map::east, zz::map::east, zz::map::east, zz::map::south, zz::map::south };
      int script[14] = { zz::map::south, zz::map::west, zz::map::west, zz::map::north, zz::map::east, zz::map::north, zz::map::west, zz::map::north, zz::map::east, zz::map::east, zz::map::south, zz::map::east, zz::map::south, zz::map::south };
      set_script( script, 14 );
    } else {
      // original script
      // int script[11] = { zz::map::south, zz::map::east, zz::map::north, zz::map::east, zz::map::south, zz::map::south, zz::map::west, zz::map::west, zz::map::west, zz::map::north, zz::map::north };
      int script[14] = { zz::map::north, zz::map::east, zz::map::east, zz::map::south, zz::map::west, zz::map::south, zz::map::east, zz::map::south, zz::map::west, zz::map::west, zz::map::north, zz::map::west, zz::map::north, zz::map::north };
      set_script( script, 14 );
    }
    return true;
  } else if( _map == _ring ) {
    if( _map.player_one( ) == zz::map::location( 5, 1 ) ) {
      int script[7] = { zz::map::east, zz::map::east, zz::map::south, zz::map::south, zz::map::south, zz::map::south, zz::map::south };
      set_script( script, 7 );
    } else {
      int script[7] = { zz::map::west, zz::map::west, zz::map::north, zz::map::north, zz::map::north, zz::map::north, zz::map::north };
      set_script( script, 7 );
    }
    return true;
  } else if( _map == _toronto ) {
//    if( _map.player_one( ) == zz::map::location( 3, 3 ) ) {
//      int script[6] = { zz::map::east, zz::map::east, zz::map::east, zz::map::east, zz::map::south, zz::map::south };
//      set_script( script, 6 );
//    } else {
//      int script[6] = { zz::map::west, zz::map::west, zz::map::west, zz::map::west, zz::map::north, zz::map::north };
//      set_script( script, 6 );
//    }
//    return true;
  } else if( _map == _trix ) {
    //    if( _map.player_one() == zz::map::location(3,3) ) {
    //      int script[6] = {1,1,1,1,2,2};
    //      set_script( script,6 );
    //    } else {
    //      int script[6] = {3,3,3,3,0,0};
    //      set_script( script, 6 );
    //    }
    //    return true;
  } else if( _map == _u ) {
    if( _map.player_one( ) == zz::map::location( 12, 2 ) ) {
      int script[14] = { zz::map::west, zz::map::west, zz::map::west, zz::map::west, zz::map::west, zz::map::west, zz::map::west, zz::map::west, zz::map::west, zz::map::south, zz::map::south, zz::map::south, zz::map::south, zz::map::south };
      set_script( script, 14 );
    } else {
      int script[14] = { zz::map::west, zz::map::west, zz::map::west, zz::map::west, zz::map::west, zz::map::west, zz::map::west, zz::map::west, zz::map::west, zz::map::north, zz::map::north, zz::map::north, zz::map::north, zz::map::north };
      set_script( script, 14 );
    }
    return true;
  }
  return false;
}

void zz::bot::set_script( int steps[], int num_steps ) {
  _script = std::list<int>( steps, steps + num_steps );
  _scripted_move = _script.begin( );
}

void zz::bot::move_default( std::map<zz::map::direction, int> directions_minimax_scores ) {
  move_first( directions_minimax_scores );
}

void zz::bot::move_first( std::map<zz::map::direction, int> directions_minimax_scores ) {
  output_move( ( *( directions_minimax_scores.begin( ) ) ).first );
}

//randomly choose a direction from the ones supplied by minimax

void zz::bot::move_random( std::map<zz::map::direction, int> directions_minimax_scores ) {
  std::map<zz::map::direction, int>::iterator it = directions_minimax_scores.begin( );
  int index = rand( ) % directions_minimax_scores.size( );
  for( int i = 0; i < index; ++i ) {
    it++;
  }
  output_move( ( *it ).first );
}

void zz::bot::move_closer( std::map<zz::map::direction, int> directions_minimax_scores, zz::map::location destination ) {
  // provide a default destination
  if( destination == zz::map::location( -1, -1 )){
    destination = zz::map::location( _map.width() / 2, _map.height() / 2 ); // center
   // destination = _map.player_two();
  }

  // find the distance for all moves from the destination
  std::map<zz::map::direction, int> moves_distances;
  for( std::map<zz::map::direction, int>::iterator it = directions_minimax_scores.begin( ); it != directions_minimax_scores.end( ); ++it ) {
    zz::map::location next_spot = _map.player_one( ).adjacent( ( *it ).first );
    moves_distances[( *it ).first] = abs( next_spot.x( ) - destination.x( ) ) + abs( next_spot.y( ) - destination.y( ) );
  }

  // collect all moves that have the minimum distance
  int min_distance = ( *( std::min_element( moves_distances.begin( ), moves_distances.end( ), map_value_comparator ) ) ).second;
  std::map<zz::map::direction, int> min_distance_moves;
  for( std::map<zz::map::direction, int>::iterator it = moves_distances.begin( ); it != moves_distances.end( ); ++it ) {
    if( ( *it ).second == min_distance ) {
      min_distance_moves[( *it ).first] = ( *it ).second;
    }
  }

  move_default( min_distance_moves );
}

void zz::bot::move_to_wall( std::map<zz::map::direction, int> directions_minimax_scores ) {
  std::map<zz::map::direction, int> moves_walls;
  for( std::map<zz::map::direction, int>::iterator it = directions_minimax_scores.begin( ); it != directions_minimax_scores.end( ); ++it ) {
    moves_walls[( *it ).first] = 0;
    zz::map::location next_spot = _map.player_one( ).adjacent( ( *it ).first );
    for( int i = 0; i < 4; ++i ) {
      if( _map.is_wall( next_spot.adjacent( i ) ) ) {
        moves_walls[( *it ).first] = moves_walls[( *it ).first] + 1;
      }
    }
  }

  int max_walls = ( *( std::max_element( moves_walls.begin( ), moves_walls.end( ), map_value_comparator ) ) ).second;
  std::map<zz::map::direction, int> max_wall_moves;
  for( std::map<zz::map::direction, int>::iterator it = moves_walls.begin( ); it != moves_walls.end( ); ++it ) {
    if( ( *it ).second == max_walls ) {
      max_wall_moves[( *it ).first] = ( *it ).second;
    }
  }

  move_default( max_wall_moves );
}

void zz::bot::move_voronoi( std::map<zz::map::direction, int> directions_minimax_scores ) {
  if( directions_minimax_scores.size() > 1 ) {
    std::cerr << " minimax: [";
    for( std::map<zz::map::direction, int>::iterator it = directions_minimax_scores.begin( ); it != directions_minimax_scores.end( ); ++it ) {
      std::cerr << ( int ) (*it).first << ":" << (int) (*it).second << ", ";
    }
    std::cerr << "]" << std::endl;

    std::map<zz::map::direction, int> moves_voronoi_territory;
    for( std::map<zz::map::direction, int>::iterator it = directions_minimax_scores.begin( ); it != directions_minimax_scores.end( ); ++it ) {
      zz::map next_map(_map);
      next_map.set_player_one( next_map.player_one().adjacent(( *it ).first) );
      moves_voronoi_territory[( *it ).first] = next_map.voronoi_territory();
    }

    int max_territory = ( *( std::max_element( moves_voronoi_territory.begin( ), moves_voronoi_territory.end( ), map_value_comparator ) ) ).second;
    std::cerr << " vornoi_territory: [";
    for( std::map<zz::map::direction, int>::iterator it = moves_voronoi_territory.begin( ); it != moves_voronoi_territory.end( ); ++it ) {
      std::cerr << ( int ) (*it).first << ":" << (int) (*it).second << ", ";
    }
    std::cerr << "] (" << ( int ) max_territory << ")" << std::endl;
    std::map<zz::map::direction, int> max_territory_moves;
    for( std::map<zz::map::direction, int>::iterator it = moves_voronoi_territory.begin( ); it != moves_voronoi_territory.end( ); ++it ) {
      if( ( *it ).second == max_territory ) {
        max_territory_moves[( *it ).first] = ( *it ).second;
      }
    }

    move_default( max_territory_moves );
  } else {
    move_default( directions_minimax_scores );
  }
}

void zz::bot::output_move( zz::map::direction direction ) {
  // The four possible moves the contest engine recognizes are:
  //   * 1 -- North. Negative Y direction.
  //   * 2 -- East. Positive X direction.
  //   * 3 -- South. Positive X direction.
  //   * 4 -- West. Negative X direction.
  int move = 1;
  switch( direction ) {
    case zz::map::north :
        std::cerr << "Moving North!" << std::endl;
        move = 1;
      break;
    case zz::map::east :
        std::cerr << "Moving East!" << std::endl;
        move = 2;
      break;
    case zz::map::south :
        std::cerr << "Moving South!" << std::endl;
        move = 3;
      break;
    case zz::map::west :
        std::cerr << "Moving West!" << std::endl;
        move = 4;
      break;
  }
  std::cout << ( int ) move << std::endl;
}
























void zz::looped_minimax(int mins[], int& max, const zz::map& map, int iterations) {
//  std::map<zz::map::direction,int> moves_scores;
//  std::map<int,zz::map> scored_maps;
//
//  std::map<zz::map,zz::map> previous_map;
//  std::set<zz::map> todo();
//  todo.insert( map );
//  while( ! todo.empty() ){
//    current_map = *todo.begin();
//    scored_maps.insert( std::pair<int, zz::map>( -1000, map ) );
//    done.insert( map );
//
//    // Find the minimax
//    int max = -1000;
//    int mins[4] = {1000, 1000, 1000, 1000};
//    for( int i = 0; i < 4; ++i ) {
//      for( int j = 0; j < 4; ++j ) {
//        if( mins[i] < max ) break;
//        zz::map::location player_one = map.player_one( ).adjacent( i );
//        zz::map::location player_two = map.player_two( ).adjacent( j );
//        bool player_one_dies = map.is_wall( player_one );
//        bool player_two_dies = map.is_wall( player_two );
//        bool collision = player_one == player_two;
//        int result;
//        if( collision || ( player_two_dies && player_one_dies ) ) {
//          // draw
//          result = -250;
//        } else if( player_two_dies && !player_one_dies ) {
//          // win
//          result = 1000;
//        } else if( !player_two_dies && player_one_dies ) {
//          // loss
//          result = -1000;
//        } else {
//          result = 0;
//        }
//      }
//      if( mins[i] > max ) {
//        max = mins[i];
//      }
//    }
//
//  }
}


void zz::minimax( int mins[], int& max, const zz::map& map, int iterations ) {
  // calculate the outcome of all possible moves by player_one
#ifdef BENCHMARK
  clock_t timer = clock( );
#endif
#ifdef DEBUG
  for( int i = 0; i < 3 - iterations; ++i ) {
    std::cerr << "  ";
  }
  std::cerr <<  map.player_one( ) << " vs " << map.player_two( ) << " minimax:" << std::endl;
#endif
  max = -1024;
  for( int i = 0; i < 4; ++i ) {
    mins[i] = 1024;
    for( int j = 0; j < 4; ++j ) {
      if( mins[i] < max ) break;
      zz::map::location player_one = map.player_one( ).adjacent( i );
      zz::map::location player_two = map.player_two( ).adjacent( j );
      bool player_one_dies = map.is_wall( player_one );
      bool player_two_dies = map.is_wall( player_two );
      bool collision = player_one == player_two;
      int result = 0;
      if( collision || ( player_two_dies && player_one_dies ) ) {
        // draw
        result = -128;
      } else if( player_two_dies && !player_one_dies ) {
        // win
        result = 1024;
      } else if( !player_two_dies && player_one_dies ) {
        // loss
        result = -1024;
      } else {
        // nothing interesting enough to end the game happened yet
        if( iterations > 0 ) {
          // look ahead at the next minimax
          zz::map next_map( map );
          next_map.set_player_one( player_one );
          next_map.set_player_two( player_two );
          int max;
          int next_scores[4];
          minimax( next_scores, max, next_map, iterations - 1 );
          result = max * 0.95;
        } else {
          result = std::min( 1024, std::max( -1024, (map.area_available_from( player_one ) - map.area_available_from( player_one )) * 256));
          // look ahead at the next available areas
//          // calculate the available areas in all 4 cardinal directions
//          int max_one = 0;
//          for( int i = 0; i < 4; ++i ) {
//            int area = next_map.area_available_from( player_one.adjacent( i ) );
//            if( area > max_one ) max_one = area;
//          }
//          // calculate the available areas in all 4 cardinal directions
//          int max_two = 0;
//          for( int i = 0; i < 4; ++i ) {
//            int area = next_map.area_available_from( player_two.adjacent( i ) );
//            if( area > max_two ) max_two = area;
//          }
//          // Considering the difference is faster than considering the ratio of the two areas, and it leads to very good fills, whereas ratios are bad at filling
//          // ratio:          result = ((float) max_one / (float)max_two * 512) - 512;
//          if( max_one > max_two ) {
//            result = std::min( 1024, 512 + ( max_one - max_two ) * 64 );
//          } else if( max_one < max_two ) {
//            result = std::max( -1024, -512 + ( max_one - max_two ) * 64 );
//          }
        }
      }
      if( result < mins[i] ) mins[i] = result;
    }
    if( mins[i] > max ) {
      max = mins[i];
    }
  }

#ifdef DEBUG
  for( int i = 0; i < 3 - iterations; ++i ) {
    std::cerr << "  ";
  }
  std::cerr << map.player_one( ) << " vs " << map.player_two( ) << " minimax mins: [";
  for( int i = 0; i < 4; ++i ) {
    std::cerr << ( int ) i << ":" << ( int ) mins[i] << ", ";
  }
  std::cerr << "] (" << ( int ) max << ")" << std::endl;
#endif
#ifdef BENCHMARK
  zz::benchmark::bot_minimax += clock( ) - timer;
#endif
}
