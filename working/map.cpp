/*
 * File:   Map.cpp
 * Author: montana
 *
 * Created on February 8, 2010, 2:49 PM
 */

#include "map.h"
#include "bot.h"

#include <limits.h>
#include <set>
#include <algorithm>

/******************************************************************************
 * Define the Location Class
 ******************************************************************************/
zz::map::location::location( )
: _x( -1 ), _y( -1 ) {
}

zz::map::location::location( coordinate x, coordinate y )
: _x( x ), _y( y ) {
}

zz::map::location::location( const location& location )
: _x( location._x ), _y( location._y ) {
}

zz::map::location::~location( ) {
}

zz::map::location zz::map::location::adjacent( zz::map::direction direction ) const {
  switch( direction ) {
    case zz::map::north :
      return north( );
    case zz::map::east :
      return east( );
    case zz::map::south :
      return south( );
    case zz::map::west :
      return west( );
  }
  std::cerr << "invalid direction provided to map::location::adjacent" << std::endl;
  exit( 1 );
}

/******************************************************************************
 * Map Construction
 ******************************************************************************/
zz::map::map( )
: _width( -1 ), _height( -1 ), _player_one( -1, -1 ), _player_two( -1, -1 ), _is_wall( ), _max( -1024 ), _mins(), _next_maps(), _next_maps_scores(), _next_maps_scored( false ), _minimaxed( false ) {
  for( int i = 0; i < 4; ++i) { _mins[i] = 1024; }
  for( int i = 0; i < 16; ++i) { _next_maps[i] = 0; }
  for( int i = 0; i < 16; ++i) { _next_maps_scores[i] = 0; }
}

zz::map::map( const zz::map& map )
: _width( map._width ), _height( map._height ), _player_one( map._player_one ), _player_two( map._player_two ), _is_wall( ), _max( -1024 ), _mins(),_next_maps(), _next_maps_scores(), _next_maps_scored( false ), _minimaxed( false ) {
  memcpy( _is_wall, map._is_wall, _width * _height * sizeof(int ) );
  for( int i = 0; i < 4; ++i) { _mins[i] = 1024; }
  for( int i = 0; i < 16; ++i) { _next_maps[i] = 0; }
  for( int i = 0; i < 16; ++i) { _next_maps_scores[i] = 0; }
}

zz::map::map( const std::string& filename )
: _width( -1 ), _height( -1 ), _player_one( -1, -1 ), _player_two( -1, -1 ), _is_wall( ), _max( -1024 ), _mins(),_next_maps(), _next_maps_scores(), _next_maps_scored( false ), _minimaxed( false ) {
  FILE* f = fopen( filename.c_str( ), "r" );
  if( f ) {
    load_from_file( f );
    fclose( f );
  }
  for( int i = 0; i < 4; ++i) { _mins[i] = 1024; }
  for( int i = 0; i < 16; ++i) { _next_maps[i] = 0; }
  for( int i = 0; i < 16; ++i) { _next_maps_scores[i] = 0; }
}

zz::map::~map( ) {
  for( zz::map** i = _next_maps; i < _next_maps + 16; ++i ) {
    if( *i ) delete *i;
  }
}

void zz::map::load_from_file( FILE *file_handle ) {
  int x, y, c;
  int num_items = fscanf( file_handle, "%d %d\n", &_width, &_height );
  if( feof( file_handle ) || num_items < 2 ) {
    exit( 0 ); // End of stream means end of game. Just exit.
  }

  x = 0;
  y = 0;
  while( y < _height && ( c = fgetc( file_handle ) ) != EOF ) {
    switch( c ) {
      case '\r':
        break;
      case '\n':
        if( x != _width ) {
          fprintf( stderr, "x != width in Board_ReadFromStream\n" );
          return;
        }
        ++y;
        x = 0;
        break;
      case '#':
        if( x >= _width ) {
          fprintf( stderr, "x >= width in Board_ReadFromStream\n" );
          return;
        }
        _is_wall[x + _width * y] = true;
        ++x;
        break;
      case ' ':
        if( x >= _width ) {
          fprintf( stderr, "x >= width in Board_ReadFromStream\n" );
          return;
        }
        _is_wall[x + _width * y] = false;
        ++x;
        break;
      case '1':
        if( x >= _width ) {
          fprintf( stderr, "x >= width in Board_ReadFromStream\n" );
          return;
        }
        _is_wall[x + _width * y] = true;
        _player_one = map::location( x, y );
        ++x;
        break;
      case '2':
        if( x >= _width ) {
          fprintf( stderr, "x >= width in Board_ReadFromStream\n" );
          return;
        }
        _is_wall[x + _width * y] = true;
        _player_two = map::location( x, y );
        ++x;
        break;
      default:
        fprintf( stderr, "unexpected character %d in Board_ReadFromStream", c );
        return;
    }
  }
}

/******************************************************************************
 * Map Query methods
 ******************************************************************************/
bool zz::map::locations_are_connected( const zz::map::location& start,  const zz::map::location& end ){
  bool end_wall = is_wall( end ); // ensure the end is not a wall, or distance_from will return no distance
  if( end_wall ) set_is_wall( end, false );
  int distance = distance_between( start, end);
  if( end_wall ) set_is_wall( end, true );

  return distance > -1;
}

int zz::map::area_available_from( const zz::map::location& location ) const {
  int area = 0;
  int counted_locations[MAP_SIZE];
  memset( counted_locations, false, MAP_SIZE * sizeof( int ) ); // initialize all locations to uncounted

  int todo[MAP_SIZE];
  todo[0] = location.x( ) + width( ) * location.y( );
  int *current_todo = todo;
  int *todo_end = todo + 1;
  counted_locations[location.x( ) + width( ) * location.y( )] = true;
  while( current_todo < todo_end ) {
    zz::map::location current( *current_todo % width( ), *current_todo / width( ) );

    if( !is_wall( current ) ) {
      ++area;

      for( int i = 0; i < 4; ++i ) {
        zz::map::location neighbor = current.adjacent( i );
        int neigbor_index = neighbor.x( ) + width( ) * neighbor.y( );
        if( !counted_locations[neigbor_index] ) {
          counted_locations[neigbor_index] = true;
          ( *todo_end++ ) = neigbor_index;
        }
      }
    }

    ++current_todo;
  }
  return area;
}

int zz::map::distance_between(const zz::map::location& start, const zz::map::location& end) {
  if( is_wall(end) ) return -1; // don't waste time with a* if it's easy to see the end is unreachable
  
  int g_scores[MAP_SIZE];
  int f_scores[MAP_SIZE];
  int came_from[MAP_SIZE];
  int done[MAP_SIZE];

  // init arrays 
  for( int* i = done; i < done + MAP_SIZE; ){ (*i++) = false; }
  for( int* i = came_from; i < came_from + MAP_SIZE; ){ (*i++) = -1; }
  for( int* i = g_scores; i < g_scores + MAP_SIZE; ){ (*i++) = INT_MAX; }
  for( int* i = f_scores; i < f_scores + MAP_SIZE; ){ (*i++) = INT_MAX; }

  // calc the starting point scores
  g_scores[start.x( ) + width( ) * start.y( )] = 0;
  f_scores[start.x( ) + width( ) * start.y( )] = manhattan_distance( start, end );

  while( true ) {
    int *min = std::min_element( f_scores, f_scores + MAP_SIZE ); // find the address of the best guess location
    if( (*min) == INT_MAX ) {
      break; // nothing left todo
    } else {
      (*min) = INT_MAX; // clear that address, so it won't be found again
    }
    int location_index = min - f_scores;
    done[location_index] = true;
    zz::map::location location ( location_index % width(), location_index / width()); // get the location from the address

    if( location == end ) {
      return zz::map::reconstruct_distance_from( came_from, location_index );
    }

    for( int i = 0; i < 4; ++i ) {
      zz::map::location neighbor = location.adjacent( i );
      int neighbor_index = neighbor.x() + width() * neighbor.y();
      if( is_wall( neighbor ) || done[neighbor_index] ) {
        continue;
      }

      if( g_scores[location_index] + 1 < g_scores[neighbor_index] ) {
        came_from[neighbor_index] = location_index;
        g_scores[neighbor_index] = g_scores[location_index] + 1;
        f_scores[neighbor_index] = g_scores[neighbor_index] + manhattan_distance( neighbor, end );
      }
    }
  }
  return -1;
}

int zz::map::reconstruct_distance_from( int came_from[], int location, int distance ) {
  int previous = came_from[location];
  if( previous > -1 ) {
    return reconstruct_distance_from( came_from, previous, distance + 1 );
  } else {
    return distance;
  }
}

int zz::map::manhattan_distance( const zz::map::location& start, const zz::map::location& end ) {
  return abs( start.x( ) - end.x( ) ) + abs( start.y( ) - end.y( ) );
}

int zz::map::voronoi_territory() {
  int player_one_territory = 0;
  int player_two_territory = 0;
  int distance_one, distance_two;
  for( int i = 0; i < width( ); ++i ) {
    for( int j = 0; j < height( ); ++j ) {
      zz::map::location location( i, j );
      distance_one = distance_between( player_one( ), location );
      distance_two = distance_between( player_two( ), location );
      if( distance_one == -1 && distance_two == -1 || distance_one == distance_two ) {
        // no mans land
      } else if( distance_one != -1 && distance_two == -1 ) {
        ++player_one_territory;
      } else if( distance_one == -1 && distance_two != -1 ) {
        ++player_two_territory;
      } else if( distance_one < distance_two ) {
        ++player_one_territory;
      } else if( distance_two < distance_one ) {
        ++player_two_territory;
      }
    }
  }
  return player_one_territory - player_two_territory;
}


int zz::map::minimax( int iterations ) {
  // calculate the outcome of all possible moves by player_one
#ifdef DEBUG
  for( int i = 0; i < 2 - iterations; ++i ) {
    std::cerr << "  ";
  }
  std::cerr <<  _player_one << " vs " << _player_two << " minimax:" << std::endl;
#endif
  for( int i = 0; i < 4; ++i ) {
    for( int j = 0; j < 4; ++j ) {
      if( _mins[i] < _max ) break;
      zz::map::location player_one = _player_one.adjacent( i );
      zz::map::location player_two = _player_two.adjacent( j );
      bool player_one_dies = is_wall( player_one );
      bool player_two_dies = is_wall( player_two );
      if( player_one == player_two || ( player_two_dies && player_one_dies ) ) {
        // draw
        _next_maps_scores[i+4*j] = -64;
      } else if( player_two_dies && !player_one_dies ) {
        // win
        _next_maps_scores[i+4*j] = 1024;
      } else if( !player_two_dies && player_one_dies ) {
        // loss
        _next_maps_scores[i+4*j] = -1024;
      } else {
        // nothing interesting enough to end the game happened yet
        if( iterations > 0 ) {
          // look ahead at the next minimax
          zz::map* next_map = _next_maps[i+4*j];
          if( ! next_map ) {
            next_map = new zz::map( *this );
            next_map->set_player_one( player_one );
            next_map->set_player_two( player_two );
            std::cerr << (*next_map) << std::endl;
            _next_maps[i+4*j] = next_map;
          }
          _next_maps_scores[i+4*j] = next_map->minimax( iterations - 1 ) * 0.95;
//          zz::map next_map( *this );
//          next_map.set_player_one( player_one );
//          next_map.set_player_two( player_two );
//          result = next_map.minimax( iterations - 1 ) * 0.95;
        } else {
          _next_maps_scores[i+4*j] = std::min( 1024, std::max( -1024, (area_available_from( player_one ) - area_available_from( player_two )) * 128));
        }
      }
      if( _next_maps_scores[i+4*j] < _mins[i] ) _mins[i] = _next_maps_scores[i+4*j];
    }
    if( _mins[i] > _max ) {
      _max = _mins[i];
    }
  }
  if( iterations > 0 )
    _next_maps_scored = true;
  
#ifdef DEBUG
  for( int i = 0; i < 2 - iterations; ++i ) {
    std::cerr << "  ";
  }
  std::cerr << player_one() << " vs " << player_two( ) << " minimax mins: [";
  for( int i = 0; i < 4; ++i ) {
    std::cerr << ( int ) i << ":" << ( int ) _mins[i] << ", ";
  }
  std::cerr << "] (" << ( int ) _max << ")" << std::endl;
#endif
  return _max;
}






bool zz::map::operator ==( const zz::map& map ) {
  return std::equal( _is_wall, _is_wall + ( _height * _width ), map._is_wall );
}

std::ostream & operator<<( std::ostream& output, const zz::map& map ) {
  for( zz::map::coordinate y = 0; y < map.height( ); ++y ) {
    for( zz::map::coordinate x = 0; x < map.width( ); ++x ) {
      zz::map::location location( x, y );
      if( map.player_one( ) == location ) {
        output << "1";
      } else if( map.player_two( ) == location ) {
        output << "2";
      } else if( map.is_wall( x, y ) ) {
        output << "#";
      } else {
        output << " ";
      }
    }
    output << std::endl;
  }
}

std::ostream & operator<<( std::ostream& output, const zz::map::location& location ) {
  output << "(" << ( int ) location.x( ) << "," << ( int ) location.y( ) << ")";
}
