/*
 * File:   Map.cpp
 * Author: montana
 *
 * Created on February 8, 2010, 2:49 PM
 */

#include "map.h"
#include <cstdlib>
#include <iostream>

/******************************************************************************
 * Define the Location Class
 ******************************************************************************/
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
  return location( *this ); // if an invalid direction is provided, fallback to this location
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

bool zz::map::location::operator<( const location& location ) const {
  return _x < location._x || ( _x == location._x && _y < location._y );
}

bool zz::map::location::operator ==( const location& location ) const {
  return _x == location._x && _y == location._y;
}






/******************************************************************************
 * Map Construction
 ******************************************************************************/
zz::map::map( )
: _width( -1 ), _height( -1 ), _player_one( -1, -1 ), _player_two( -1, -1 ), _is_wall( ) {
}

zz::map::map( const zz::map& map )
: _width( map._width ), _height( map._height ), _player_one( map._player_one ), _player_two( map._player_two ), _is_wall( ) {
  memcpy( &_is_wall, &( map._is_wall ), _width * _height * sizeof (int ) );
}

zz::map::~map( ) {
}

void zz::map::load_from_file( FILE *file_handle ) {
  int num_items = fscanf( file_handle, "%d %d\n", &_width, &_height );
  if( feof( file_handle ) || num_items < 2 ) {
    exit( 0 ); // End of stream means end of game. Just exit.
  }

  int c;
  for( zz::map::coordinate y = 0; y < _height; ++y ) {
    for( zz::map::coordinate x = 0; x < _width; ++x ) {
      std::cerr << ( int ) x << "," << ( int ) y << std::endl;
      if( ( c = fgetc( file_handle ) ) != EOF ) {
        switch( c ) {
          case '\r':
            --x; // eol chars don't count as part of the board
            break;
          case '\n':
            --x; // eol chars don't count as part of the board
            break;
          case '#':
            _is_wall[x + _width * y] = true;
            break;
          case ' ':
            _is_wall[x + _width * y] = false;
            break;
          case '1':
            _is_wall[x + _width * y] = true;
            _player_one = zz::map::location( x, y );
            break;
          case '2':
            _is_wall[x + _width * y] = true;
            _player_two = zz::map::location( x, y );
            break;
          default:
            std::cerr << "unexpected character '" << c << "' while loading the game board" << std::endl;
            return;
        }
      } else {
        std::cerr << "The stream ran out, before the board was complete!" << std::endl;
        return;
      }
    }
  }
}

int zz::map::width( ) const {
  return _width;
}

int zz::map::height( ) const {
  return _height;
}

bool zz::map::is_wall( const map::location& location ) const {
  return is_wall( location.x( ), location.y( ) );
}

bool zz::map::is_wall( zz::map::coordinate x, zz::map::coordinate y ) const {
  return x < 0 || y < 0 || x >= _width || y >= _height || _is_wall[x + _width * y];
}

const zz::map::location& zz::map::player_one( ) const {
  return _player_one;
}

const zz::map::location& zz::map::player_two( ) const {
  return _player_two;
}

void zz::map::set_player_one( map::location location ) {
  _player_one = location;
  _is_wall[_player_one.x( ) + _width * _player_one.y( )] = true;
}

void zz::map::set_player_two( map::location location ) {
  _player_two = location;
  _is_wall[_player_two.x( ) + _width * _player_two.y( )] = true;
}

int zz::map::area_available_from( zz::map::location location, int* counted_locations ) {
  if( is_wall( location ) ) return 0;

  // We'll need to keep track of all previously counted locations, so we don't double count them
  bool cleanup = false;
  if( !counted_locations ) {
    cleanup = true;
    int size = _width * _height;
    counted_locations = new int[size];
    memset( counted_locations, 0, size * sizeof (int ) ); // initialize all values to 0
  };

  int available_area = 1;
  for( zz::map::direction i = 0; i < 4; ++i ) {
    zz::map::location destination = location.adjacent( i );
    int index = destination.x( ) + _width * destination.y( );
    if( !counted_locations[index] ) {
      counted_locations[index] = true;
      available_area += area_available_from( destination, counted_locations );
    }
  }

  if( cleanup ) {
    delete[] counted_locations;
  }

  return available_area;
}

std::ostream& operator<<( std::ostream& output, zz::map& map ) {
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

