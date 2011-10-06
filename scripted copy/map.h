/*
 * File:   Map.h
 * Author: montana
 *
 * Created on February 8, 2010, 2:49 PM
 */

#ifndef _MAP_H
#define	_MAP_H

#include <cstdlib>
#include <iostream>

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
      location(coordinate x, coordinate y);
      location(const location& location);
      ~location();

      inline const coordinate x() const;
      inline const coordinate y() const;

      // Returns the adjacent location in the direction from this location
      location adjacent( zz::map::direction direction ) const;
      // Shorthand calls to adjacent
      inline location north() const;
      inline location east() const;
      inline location south() const;
      inline location west() const;

      inline bool operator < (const location& location ) const;
      inline bool operator == (const location& location ) const;
    private:
      coordinate _x;
      coordinate _y;
    };



    /**************************************************************************
    * The map
    **************************************************************************/

    // Constructs a map by reading an ASCII representation from the console (stdin).
    map();

    // Constructs a map by deep copying the fields from other.
    map(const zz::map& other);

    ~map();
    
    // Returns the width of the Tron map.
    inline int width() const;

    // Returns the height of the Tron map.
    inline int height() const;

    inline const zz::map::location& player_one() const;
    inline const zz::map::location& player_two() const;

    // Returns whether or not the given cell is a wall or not. TRUE means it's
    // a wall, FALSE means it's not a wall, and is passable. Any spaces that are
    // not on the board are deemed to be walls.
    inline bool is_wall(const zz::map::location& location) const;
    inline bool is_wall(zz::map::coordinate x, zz::map::coordinate y) const;

    // Returns the number of squares that are reachable from the location.  If
    // the location is a wall, returns 0.
    int area_available_from(zz::map::location location, int *counted_locations = 0);

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
    void load_from_file(FILE *file_handle);

    friend std::ostream& operator<<(std::ostream& output, zz::map& map);

  private:
    void set_player_one(zz::map::location location);
    void set_player_two(zz::map::location location);


    // Map dimensions.
    zz::map::coordinate _width, _height;

    // The locations of both players.
    zz::map::location _player_one;
    zz::map::location _player_two;

    // Indicates whether or not each cell in the board is passable.
    int _is_wall[225];
  };

}

#endif // _MAP_H