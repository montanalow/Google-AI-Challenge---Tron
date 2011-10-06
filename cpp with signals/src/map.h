/*
 * File:   map.h
 * Author: montana
 *
 * Created on February 8, 2010, 2:49 PM
 */

#ifndef _ZZ_MAP_H
#define	_ZZ_MAP_H

#define MAP_SIZE_MAX 2500

#include <stdio.h>

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
      location( zz::map::coordinate x, zz::map::coordinate y );
      location( const location& location );
      ~location( );


      const zz::map::coordinate x( ) const;
      const zz::map::coordinate y( ) const;

      // Returns the adjacent location in the direction from this location
      location adjacent( zz::map::direction direction ) const;
      location north( ) const;
      location east( ) const;
      location south( ) const;
      location west( ) const;

      operator bool( ) const;

      // Equality checks
      bool operator <(const location& location ) const;
      bool operator ==(const location & location ) const;
      bool operator !=(const location & location ) const;

    private:
      zz::map::coordinate _x;
      zz::map::coordinate _y;
    };


    /**************************************************************************
     * The map
     **************************************************************************/
    // Constructs a new empty map.  This is generally used for 2 phase construction with load_from_file
    map( );

    // Constructs a map by deep copying the fields from another.
    map( const zz::map& map );
    map( const zz::map& map, const zz::map::location& center, int width, int height );

    // Constructs a map by reading an ASCII representation from the file.
    map( const char* filename );

    ~map( );

    // Returns the width of the Tron map.
    int width( ) const;

    // Returns the height of the Tron map.
    int height( ) const;

    const zz::map::location& player_one( ) const;
    const zz::map::location& player_two( ) const;

    void set_player_one( const zz::map::location& location );
    void set_player_two( const zz::map::location& location );

    // Returns whether or not the given cell is a wall or not. TRUE means it's
    // a wall, FALSE means it's not a wall, and is passable. Any spaces that are
    // not on the board are deemed to be walls.
    const bool is_wall( const zz::map::location& location ) const;
    const bool is_wall( zz::map::coordinate x, zz::map::coordinate y ) const;

    void set_is_wall( const zz::map::location& location, bool value = true );

    int is_chamber( const zz::map::location& location ) const;
    int is_door( const zz::map::location& location ) const;

    const int max( ) const;
    const int (&mins( ) const)[4]; // const int[4] mins( ) const

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

    // returns the shortest possible distance between two locations.  If the end point
    // is a wall (or a player), -1 will be returned.  If there is no valid path
    // between the two locations, -1 will be returned.
    int distance_between( const zz::map::location& start, const zz::map::location& end ) const;

    // returns the Manhattan distance between 2 locations.
    int manhattan_distance( const zz::map::location& start, const zz::map::location& end ) const;

    // returns the number of squares reachable from location
    int area_available_from( const zz::map::location& location ) const;

    // returns true if there is an open path between start and end, even if start and end are
    // both walls themselves, otherwise, false.
    bool locations_are_connected( const zz::map::location& start, const zz::map::location& end ) const;

    // returns the difference in Voronoi territory between player_one and player_two.
    // A positive result indicates that player_one has the larger territory
    // A negative result indicates that player_two has the larger territory
    int territory( ) const;

    int minimax( int iterations = 0, bool use_heuristic = true );
    int solomax( int iterations = 0, bool use_heuristic = true );

    bool operator == ( const zz::map& map ) const;

    static int __minimax_leafs;


  private:
    int reconstruct_distance_from( int came_from[], int location, int distance = 0 ) const;
    void set_is_wall( const zz::map::location& location, bool value = true ) const;
    const bool is_wall( zz::map::coordinate x_width_y ) const;

    int _max;
    int _mins[4];

    // Map dimensions.
    zz::map::coordinate _width, _height;

    // The locations of both players.
    zz::map::location _player_one;
    zz::map::location _player_two;

    // Indicates whether or not each cell in the board is passable.
    mutable int _locations[MAP_SIZE_MAX]; // sometimes we need to muck with where walls are, so that we don't have to deep copy, when we just need 1 wall changed
  };
};

#endif // _ZZ_MAP_H
