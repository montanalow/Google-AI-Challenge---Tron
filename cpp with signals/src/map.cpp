/*
 * File:   Map.cpp
 * Author: montana
 *
 * Created on February 8, 2010, 2:49 PM
 */

#include "map.h"
#include "bot.h"
#include "config.h"

#include <limits.h>
#include <algorithm>

int zz::map::__minimax_leafs = 0;

/******************************************************************************
 * zz::map::location
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

const zz::map::coordinate zz::map::location::x( ) const {
  return _x;
}

const zz::map::coordinate zz::map::location::y( ) const {
  return _y;
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
  fprintf( stderr, "invalid direction provided to map::location::adjacent\n" );
  fflush( stderr );
  exit( EXIT_FAILURE );
}

zz::map::location zz::map::location::north( ) const {
  return location( _x, _y - 1 );
}

zz::map::location zz::map::location::east( ) const {
  return location( _x + 1, _y );
}

zz::map::location zz::map::location::south( ) const {
  return location( _x, _y + 1 );
}

zz::map::location zz::map::location::west( ) const {
  return location( _x - 1, _y );
}

zz::map::location::operator bool( ) const {
  return _x != -1 || _y != -1;
}

bool zz::map::location::operator <(const location& location ) const {
  return _x < location._x || ( _x == location._x && _y < location._y );
}

bool zz::map::location::operator ==(const location & location ) const {
  return _x == location._x && _y == location._y;
};

bool zz::map::location::operator !=(const location & location ) const {
  return !( ( *this ) == location );
};


/******************************************************************************
 * zz::map
 ******************************************************************************/
zz::map::map( )
: _max( INT_MIN ), _mins( ), _width( -1 ), _height( -1 ), _player_one( -1, -1 ), _player_two( -1, -1 ), _locations( ) {
}

zz::map::map( const zz::map& map )
: _max( INT_MIN ), _mins( ), _width( map._width ), _height( map._height ), _player_one( map._player_one ), _player_two( map._player_two ), _locations( ) {
  int* location = _locations;
  int* other = map._locations;
  for( int i = 0; i < _width * _height; ) { (*location++) = (*other++); }
}

zz::map::map( const zz::map& map, const zz::map::location& center, int width, int height )
: _max( INT_MIN ), _mins( ), _width( width ), _height( height ), _player_one( -1, -1 ), _player_two( -1, -1 ), _locations( ) {
  for( int i = 0; i < _width; ++i ) {
    for( int j = 0; j < _height; ++j ) {
      int x = center.x( ) - ( _width / 2 ) + i;
      int y = center.y( ) - ( _height / 2 ) + j;
      _locations[i + _width * j] = x >= map.width( ) || y >= map.height( ) || map.is_wall(x,y);
    }
  }
  set_player_one( zz::map::location( map.player_one( ).x( ) - ( center.x( ) - ( _width / 2 ) ), map.player_one( ).y( ) - ( center.y( ) - ( _height / 2 ) ) ) );
  set_player_two( zz::map::location( map.player_two( ).x( ) - ( center.x( ) - ( _width / 2 ) ), map.player_two( ).y( ) - ( center.y( ) - ( _height / 2 ) ) ) );
}

zz::map::map( const char* filename )
: _max( INT_MIN ), _mins( ), _width( -1 ), _height( -1 ), _player_one( -1, -1 ), _player_two( -1, -1 ), _locations( ) {
  FILE* f = fopen( filename, "r" );
  if( f ) {
    load_from_file( f );
    fclose( f );
  } else {
    fprintf( stderr, "could not open map: %s\n", filename );
    fflush( stderr );
    exit( EXIT_FAILURE );
  }
}

zz::map::~map( ) {
}

void zz::map::load_from_file( FILE *file_handle ) {
  int x, y, c;
  int num_items = fscanf( file_handle, "%d %d\n", &_width, &_height );
  for( int i = 0; i < _width * _height; ++i ) { _locations[i] = 0; }
  if( feof( file_handle ) || num_items < 2 ) {
    exit( 0 ); // End of stream means end of game. Just exit.
  }

  x = 0;
  y = 0;
  while( y < height() && ( c = fgetc( file_handle ) ) != EOF ) {
    switch( c ) {
      case '\r':
        break;
      case '\n':
        if( x != width() ) {
          fprintf( stderr, "x != width in Board_ReadFromStream\n" );
          exit( EXIT_FAILURE );
        }
        ++y;
        x = 0;
        break;
      case '#':
        if( x >= width() ) {
          fprintf( stderr, "x >= width in Board_ReadFromStream\n" );
          exit( EXIT_FAILURE );
        }
        set_is_wall( zz::map::location(x, y) );
        ++x;
        break;
      case ' ':
        if( x >= width() ) {
          fprintf( stderr, "x >= width in Board_ReadFromStream\n" );
          exit( EXIT_FAILURE );
        }
        ++x;
        break;
      case '1':
        if( x >= width() ) {
          fprintf( stderr, "x >= width in Board_ReadFromStream\n" );
          exit( EXIT_FAILURE );
        }
        set_player_one( zz::map::location(x, y ) );
        ++x;
        break;
      case '2':
        if( x >= width() ) {
          fprintf( stderr, "x >= width in Board_ReadFromStream\n" );
          exit( EXIT_FAILURE );
        }
        set_player_two( zz::map::location(x, y ) );
        ++x;
        break;
      default:
        fprintf( stderr, "unexpected character %d in Board_ReadFromStream", c );
        exit( EXIT_FAILURE );
    }
  }
}

/******************************************************************************
 * Map Query methods
 ******************************************************************************/
int zz::map::width( ) const {
  return _width;
}

int zz::map::height( ) const {
  return _height;
}

const zz::map::location& zz::map::player_one( ) const {
  return _player_one;
}

const zz::map::location& zz::map::player_two( ) const {
  return _player_two;
}

void zz::map::set_player_one( const zz::map::location& location ) {
  _player_one = location;
  set_is_wall( location );
}

void zz::map::set_player_two( const zz::map::location& location ) {
  _player_two = location;
  set_is_wall( location );
}

// Returns whether or not the given cell is a wall or not. TRUE means it's
// a wall, FALSE means it's not a wall, and is passable. Any spaces that are
// not on the board are deemed to be walls.
const bool zz::map::is_wall( const zz::map::location& location ) const {
  return is_wall( location.x(), location.y() );
}

const bool zz::map::is_wall( zz::map::coordinate x, zz::map::coordinate y ) const {
  return is_wall( x + width() * y );
}

const bool zz::map::is_wall( zz::map::coordinate x_width_y ) const {
  return _locations[x_width_y] < 0;
}

void zz::map::set_is_wall( const zz::map::location& location, bool value ) {
  ((const zz::map*) this)->set_is_wall( location, value );
}

void zz::map::set_is_wall( const zz::map::location& location, bool value ) const {
  if( value ) {
    _locations[location.x( ) + width() * location.y( )] = -1;
  } else {
    _locations[location.x( ) + width() * location.y( )] = 0;
  }
};

const int zz::map::max( ) const {
  return _max;
}

const int (&zz::map::mins( ) const)[4] {
  return _mins;
}

bool zz::map::locations_are_connected( const zz::map::location& start, const zz::map::location& end ) const {
  bool end_wall = is_wall( end ); // ensure the end is not a wall, or distance_from will return no distance
  if( end_wall ) set_is_wall( end, false );

  int distance = distance_between( start, end );
  
  if( end_wall ) set_is_wall( end );// reset the end wall if we mucked with it

  return distance != -1;
}

int zz::map::area_available_from( const zz::map::location& location ) const {
  if( is_wall( location) ){ return 0; }
  int area = 0;
  int counted_locations[width() * height()];
  for( int* i = counted_locations; i < counted_locations + width() * height(); ) { ( *i++ ) = false; }

  int todo[width() * height()];
  todo[0] = location.x( ) + width( ) * location.y( );
  int *current_todo = todo;
  int *todo_end = todo + 1;
  counted_locations[location.x( ) + width( ) * location.y( )] = true;
  while( current_todo < todo_end ) {
    zz::map::location current( *current_todo % width( ), *current_todo / width( ) );

    ++area;

    for( int i = 0; i < 4; ++i ) {
      zz::map::location neighbor = current.adjacent( i );
      int neighbor_index = neighbor.x( ) + width( ) * neighbor.y( );
      if( !counted_locations[neighbor_index] && !is_wall(neighbor_index) ) {
        counted_locations[neighbor_index] = true;
        ( *todo_end++ ) = neighbor_index;
      }
    }

    ++current_todo;
  }
  return area;
}

int zz::map::distance_between( const zz::map::location& start, const zz::map::location& end ) const {
  if( is_wall( end ) ) return -1; // don't waste time with A* if it's easy to see the end is unreachable

  int g_scores[width() * height()];
  int f_scores[width() * height()];
  int came_from[width() * height()];
  int done[width() * height()];

  // init arrays 
  for( int* i = done; i < done + width() * height(); ) { ( *i++ ) = false; }
  for( int* i = came_from; i < came_from + width() * height(); ) { ( *i++ ) = -1; }
  for( int* i = g_scores; i < g_scores + width() * height(); ) { ( *i++ ) = INT_MAX; }
  for( int* i = f_scores; i < f_scores + width() * height(); ) { ( *i++ ) = INT_MAX; }

  // calc the starting point scores
  g_scores[start.x( ) + width( ) * start.y( )] = 0;
  f_scores[start.x( ) + width( ) * start.y( )] = manhattan_distance( start, end );

  // Use a basic A* implementation to calculate the shortest path
  while( true ) {
    int *min = std::min_element( f_scores, f_scores + width() * height() ); // find the address of the best guess location
    if( ( *min ) == INT_MAX ) {
      break; // nothing left todo
    } else {
      ( *min ) = INT_MAX; // clear that address, so it won't be found again
    }
    int location_index = min - f_scores;
    done[location_index] = true;
    zz::map::location location( location_index % width( ), location_index / width( ) ); // get the location from the address

    if( location == end ) { // found the destination, so reconstruct the path to here
      return zz::map::reconstruct_distance_from( came_from, location_index );
    }

    for( int i = 0; i < 4; ++i ) {
      zz::map::location neighbor = location.adjacent( i );
      int neighbor_index = neighbor.x( ) + width( ) * neighbor.y( );
      if( is_wall( neighbor_index ) || done[neighbor_index] ) {
        continue;
      }

      if( g_scores[location_index] + 1 < g_scores[neighbor_index] ) {
        came_from[neighbor_index] = location_index;
        g_scores[neighbor_index] = g_scores[location_index] + 1;
        f_scores[neighbor_index] = g_scores[neighbor_index] + manhattan_distance( neighbor, end );
      }
    }
  }
  return -1;  // A* could not find a path between start and end
}

int zz::map::reconstruct_distance_from( int came_from[], int location, int distance ) const {
  int previous = came_from[location];
  if( previous > -1 ) {
    return reconstruct_distance_from( came_from, previous, distance + 1 );
  } else {
    return distance;
  }
}

int zz::map::manhattan_distance( const zz::map::location& start, const zz::map::location& end ) const {
  return abs( start.x( ) - end.x( ) ) + abs( start.y( ) - end.y( ) );
}

int zz::map::territory( ) const {
  int player_one_territory = 0;
  int player_two_territory = 0;
  int player_one_distances[width() * height()];
  int player_two_distances[width() * height()];
  for( int* i = player_one_distances; i < player_one_distances + width() * height(); ) { ( *i++ ) = -1; }
  for( int* i = player_two_distances; i < player_two_distances + width() * height(); ) { ( *i++ ) = -1; }
  int todo[width() * height()];
  int counted_locations[width() * height()];

  // initialize everything for player_one
  for( int* i = counted_locations; i < counted_locations + width() * height(); ) { ( *i++ ) = false; }
  int* current_todo = todo;
  int* todo_end = todo;
  (*todo_end++) = player_one().x() + width() * player_one().y();
  counted_locations[player_one().x( ) + width( ) * player_one().y( )] = true;
  player_one_distances[player_one().x( ) + width( ) * player_one().y( )] = 0;
  // find the distance to every location player_one can reach
  while( current_todo < todo_end ) {
    zz::map::location current( *current_todo % width( ), *current_todo / width( ) );
    ++player_one_territory; // everything will belong to player one, until we start counting player_two
    int current_index = current.x( ) + width() * current.y( );
    int distance = player_one_distances[current_index];
    for( int i = 0; i < 4; ++i ) {
      // add all accessible neighbors of this location to the todo list
      zz::map::location neighbor = current.adjacent( i );
      int neighbor_index = neighbor.x( ) + width( ) * neighbor.y( );
      if( !counted_locations[neighbor_index] && !is_wall(neighbor_index) ) {
        player_one_distances[neighbor_index] = distance + 1;
        counted_locations[neighbor_index] = true;
        ( *todo_end++ ) = neighbor_index;
      }
    }
    ++current_todo;
  }


  // re-init for player_two
  for( int* i = counted_locations; i < counted_locations + width() * height(); ) { ( *i++ ) = false; }
  current_todo = todo;
  todo_end = todo;
  (*todo_end++) = player_two().x() + width() * player_two().y();
  counted_locations[player_two().x( ) + width( ) * player_two().y( )] = true;
  player_two_distances[player_two().x( ) + width( ) * player_two().y( )] = 0;
  // find the distance to every location player_one can reach and adjust ownership of contested locations
  while( current_todo < todo_end ) {
    zz::map::location current( *current_todo % width( ), *current_todo / width( ) );
    int current_index = current.x( ) + width() * current.y( );
    int distance = player_two_distances[current_index];
    int player_one_distance = player_one_distances[current_index];
    if( player_one_distance == -1 || distance < player_one_distance ) {
      ++player_two_territory; // everything will belong to player one, until we start counting player_two
    }
    if ( distance < player_one_distance || distance == player_one_distance) {
      --player_one_territory; // everything will belong to player one, until we start counting player_two
    }
    for( int i = 0; i < 4; ++i ) {
      // add all accessible neighbors of this location to the todo list
      zz::map::location neighbor = current.adjacent( i );
      int neighbor_index = neighbor.x( ) + width( ) * neighbor.y( );
      if( !counted_locations[neighbor_index] && !is_wall(neighbor_index) ) {
        player_two_distances[neighbor_index] = distance + 1;
        counted_locations[neighbor_index] = true;
        ( *todo_end++ ) = neighbor_index;
      }
    }
    ++current_todo;
  }

  return player_one_territory - player_two_territory;
}


int zz::map::minimax( int iterations, bool use_heuristic ) {
#ifdef DEBUG_MAP
  fprintf( stderr, "%*s(%d,%d) vs (%d,%d) minimax:\n", ( 10 - iterations ) * 2, "", player_one().x( ), player_one().y( ), player_two().x( ), player_two().y( ) );
#endif
  // calculate the outcome of all possible moves of player_one and player_two
  _max = INT_MIN;
  for( int i = 0; i < 4; ++i ) {
    _mins[i] = INT_MAX;
    for( int j = 0; j < 4; ++j ) {
      if( _mins[i] < _max ) break;
      zz::map::location player_one = _player_one.adjacent( i );
      zz::map::location player_two = _player_two.adjacent( j );
      bool player_one_dies = is_wall( player_one );
      bool player_two_dies = is_wall( player_two );
      int result = 0;
      if( player_one == player_two || ( player_two_dies && player_one_dies ) ) {
        // draw
        result = -64;
      } else if( player_two_dies && !player_one_dies ) {
        // win
        result = INT_MAX;
      } else if( !player_two_dies && player_one_dies ) {
        // loss
        result = INT_MIN;
      } else if( !use_heuristic && iterations <= 0 ) {
        result = 0;
        ++__minimax_leafs;
      } else {
        // nothing interesting enough to end the game happened yet
        // save the current state
        int max = _max;
        int mins[4];
        for( int k = 0; k < 4; ++k ){ mins[k] = _mins[k]; }
        zz::map::location player_one_original( _player_one );
        zz::map::location player_two_original( _player_two );
        // update the map
        set_player_one( player_one );
        set_player_two( player_two );
        if( iterations > 0 ) {
          // get the next minimax
          result = minimax( iterations - 1, use_heuristic ) * 0.999;
        } else {
          // get the voronoi_territory
          result = territory() * 128;
          ++__minimax_leafs;
        }
        // restore the current state
        set_player_one( player_one_original );
        set_player_two( player_two_original );
        set_is_wall(player_one, false);
        set_is_wall(player_two, false);
        _max = max;
        for( int k = 0; k < 4; ++k ){ _mins[k] = mins[k]; }
      }
      if( result < _mins[i] ) _mins[i] = result;
    }
    if( _mins[i] > _max ) {
      _max = _mins[i];
    }
  }

#ifdef DEBUG_MAP
  fprintf( stderr, "%*s(%d,%d) vs (%d,%d) minimax.mins(): [", ( 10 - iterations ) * 2, "", player_one().x( ), player_one().y( ), player_two().x( ), player_two().y( ) );
  for( int i = 0; i < 4; ++i ) {
    fprintf( stderr, "%d:%d", i, _mins[i] );
    if( i != 3 ) fprintf( stderr, ", " );
  }
  fprintf( stderr, "] (%d)\n", _max );
#endif
  return _max;
}


int zz::map::solomax( int iterations, bool use_heuristic ) {
#ifdef DEBUG_MAP
  fprintf( stderr, "%*s(%d,%d) vs (%d,%d) solomax:\n", ( 10 - iterations ) * 2, "", player_one().x( ), player_one().y( ), player_two().x( ), player_two().y( ) );
#endif
  _max = INT_MIN;
  for( int i = 0; i < 4; ++i ) {
    zz::map::location player_one = _player_one.adjacent( i );
    if( is_wall( player_one ) ) {
      _mins[i] = INT_MIN;
    } else {
      if( iterations > 0 ) {
        // look ahead at the next solomax
        int result = 0;
        // save the current state
        int max = _max;
        int mins[4];
        for( int k = 0; k < 4; ++k ){ mins[k] = _mins[k]; }
        zz::map::location player_one_original( _player_one );
        // update the map
        set_player_one( player_one );
        // get the next solomax
        result = solomax( iterations - 1, use_heuristic ) * 0.999;
        // restore the current state
        set_player_one( player_one_original );
        set_is_wall( player_one, false );
        _max = max;
        for( int k = 0; k < 4; ++k ){ _mins[k] = mins[k]; }
        _mins[i] = result;
      } else {
        ++__minimax_leafs;
        if( use_heuristic ) {
          _mins[i] = area_available_from( player_one ) * 128;
        } else {
          _mins[i] = 0;
        }
      }
    }
    if( _mins[i] > _max ) {
      _max = _mins[i];
    }
  }
#ifdef DEBUG_MAP
  fprintf( stderr, "%*s(%d,%d) vs (%d,%d) solomax.mins(): [", ( 10 - iterations ) * 2, "", player_one().x( ), player_one().y( ), player_two().x( ), player_two().y( ) );
  for( int i = 0; i < 4; ++i ) {
    fprintf( stderr, "%d:%d", i, _mins[i] );
    if( i != 3 ) fprintf( stderr, ", " );
  }
  fprintf( stderr, "] (%d)\n", _max );
#endif
  return _max;
}

bool zz::map::operator == ( const zz::map& map ) const {
  return std::equal( _locations, _locations + ( _height * _width ), map._locations );
}
