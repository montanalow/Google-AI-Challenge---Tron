/*
 * File:   map.h
 * Author: montana
 *
 * Created on February 8, 2010, 2:49 PM
 */

#ifndef _ZZ_MAP_H
#define	_ZZ_MAP_H

#include <stdio.h>
#include <string.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <map>

#define MAP_SIZE 625

namespace zz {

  class map;

  class map {

  public:
    /**************************************************************************
     * Maps have lots of coordinates
     **************************************************************************/
    typedef int coordinate;


    /**************************************************************************
     * The 4 Cardinal Directions
     **************************************************************************/
    typedef int direction;
    static const int north = 0;
    static const int east = 1;
    static const int south = 2;
    static const int west = 3;

    /**************************************************************************
     * Map locations consist of an x and y coordinate
     **************************************************************************/
    class location {

    public:
      location( );
      location( coordinate x, coordinate y );
      location( const location& location );
      ~location( );

      inline const coordinate x( ) const {
        return _x;
      };

      inline const coordinate y( ) const {
        return _y;
      };

      // Shorthand calls to adjacent

      inline location north( ) const {
        return location( _x, _y - 1 );
      };

      inline location east( ) const {
        return location( _x + 1, _y );
      };

      inline location south( ) const {
        return location( _x, _y + 1 );
      };

      inline location west( ) const {
        return location( _x - 1, _y );
      };
      // Returns the adjacent location in the direction from this location
      location adjacent( zz::map::direction direction ) const;


      // Equality checks

      inline bool operator<(const location& location ) const {
        return _x < location._x || ( _x == location._x && _y < location._y );
      }

      inline bool operator ==(const location & location ) const {
        return _x == location._x && _y == location._y;
      };

      inline bool operator !=(const location & location ) const {
        return !( ( *this ) == location );
      };

    private:
      coordinate _x;
      coordinate _y;
    };



    /**************************************************************************
     * The map
     **************************************************************************/

    // Constructs a map by reading an ASCII representation from the console (stdin).
    map( );

    // Constructs a map by deep copying the fields from other.
    map( const zz::map& other );

    map( const std::string& filename );
    ~map( );

    // Returns the width of the Tron map.

    inline int width( ) const {
      return _width;
    };

    // Returns the height of the Tron map.

    inline int height( ) const {
      return _height;
    };

    inline const zz::map::location& player_one( ) const {
      return _player_one;
    };

    inline const zz::map::location& player_two( ) const {
      return _player_two;
    };

    inline void set_player_one( const zz::map::location& location ) {
      _player_one = location;
      _is_wall[_player_one.x( ) + _width * _player_one.y( )] = true;
    };

    inline void set_player_two( const zz::map::location& location ) {
      _player_two = location;
      _is_wall[_player_two.x( ) + _width * _player_two.y( )] = true;
    }

    // Returns whether or not the given cell is a wall or not. TRUE means it's
    // a wall, FALSE means it's not a wall, and is passable. Any spaces that are
    // not on the board are deemed to be walls.

    inline bool is_wall( const zz::map::location& location ) const {
      return is_wall( location.x( ), location.y( ) );
    };

    inline bool is_wall( zz::map::coordinate x, zz::map::coordinate y ) const {
      return x < 0 || y < 0 || x >= _width || y >= _height || _is_wall[x + _width * y];
    };

    inline void set_is_wall( const zz::map::location& location, bool value = true ) {
      if( location.x( ) >= 0 && location.y( ) >= 0 && location.x( ) < _width && location.y( ) < _height )
        _is_wall[location.x( ) + _width * location.y( )] = value;
    }


    // Load a board from an open file handle. To read from the console, pass
    // stdin, which is actually a (FILE*).
    //   file_handle -- an open file handle from which to read.
    //
    // If there is a problem, the function returns NULL. Otherwise, a valid
    // Board structure is returned.
    //
    // The file should be an ascii file. The first line contains the width and
    // height of the board, separated by a space. subsequent lines contain visual
    // representations of the rows of the board, using '#' and space characters.
    // The starting positions of the two players are indicated by '1' and '2'
    // characters. There must be exactly one '1' character and one '2' character
    // on the board. For example:
    // 6 4
    // ######
    // #1# 2#
    // #   ##
    // ######
    void load_from_file( FILE *file_handle );

    bool operator ==(const zz::map& map );

    // returns the shortest possible distance between two locations.  If the end point
    // is a wall (or a player), -1 will be returned.  If there is no valid path
    // between the two locations, -1 will be returned.
    int distance_between( const zz::map::location& start, const zz::map::location& end );

    // returns the Manhattan distance between 2 locations.
    int manhattan_distance( const zz::map::location& start, const zz::map::location& end );

    // returns the number of squares reachable from location
    int area_available_from( const zz::map::location& location ) const;

    // returns true if there is an open path between start and end, even if start and end are
    // both walls themselves.
    bool locations_are_connected( const zz::map::location& start, const zz::map::location& end );

    // returns the difference in Voronoi territory between player_one and player_two.
    // A positive result indicates that player_one has the larger territory
    // A negative result indicates that player_two has the larger territory
    int voronoi_territory( );

    int minimax( int iterations = 0 );

    int _mins[4];

  private:
    int reconstruct_distance_from( int came_from[], int location, int distance = 0 );

    int _max;
    int _next_maps_scores[16];
    int _next_maps_scored;
    int _minimaxed;
    int _next_moves_order[4];
    zz::map* _next_maps[16];
    
    // Map dimensions.
    zz::map::coordinate _width, _height;

    // The locations of both players.
    zz::map::location _player_one;
    zz::map::location _player_two;

    // Indicates whether or not each cell in the board is passable.
    int _is_wall[MAP_SIZE];
  };
};

std::ostream & operator<<( std::ostream& output, const zz::map& map );
std::ostream & operator<<( std::ostream& output, const zz::map::location& location );
#endif // _ZZ_MAP_H
