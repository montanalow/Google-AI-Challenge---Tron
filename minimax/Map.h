// Map.h
//
// Handles the Tron map. Also handles communicating with the Tron game engine.
// You don't need to change anything in this file.

#include <vector>
#include <map>
#include <set>
#include <utility>
#include <iostream>

namespace zz {

  class map;

  class map {
  public:

    typedef unsigned char direction;
    static const direction north;
    static const direction east;
    static const direction south;
    static const direction west;

    typedef std::pair<unsigned char, unsigned char> location;

    // Constructs a map by reading an ASCII representation from the console (stdin).
    map();

    // Constructs a map by deep copying the fields from other.
    map(const zz::map& other);

    // Returns the width of the Tron map.
    int width() const;

    // Returns the height of the Tron map.
    int height() const;

    // Returns whether or not the given cell is a wall or not. TRUE means it's
    // a wall, FALSE means it's not a wall, and is passable. Any spaces that are
    // not on the board are deemed to be walls.
    bool is_wall(const map::location& location) const;
    bool is_wall(int x, int y) const;

    void make_move();

  private:
    void set_valid_moves();

    // Sends your move to the contest engine. The four possible moves are
    //   * 1 -- North. Negative Y direction.
    //   * 2 -- East. Positive X direction.
    //   * 3 -- South. Positive X direction.
    //   * 4 -- West. Negative X direction.
    void make_move(const zz::map::direction direction);

    std::map<zz::map::direction, std::pair<float, int> > evaluate_moves(int depth = 0);

    map::location move_from(const map::location& location, const map::direction direction);
    void set_player_one(map::location location);
    void set_player_two(map::location location);
    std::vector<direction> get_open_directions_for(map::location location);
    int area_available_from( zz::map::location location, std::set<map::location>* counted_locations = 0);

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
    void read_from_file(FILE *file_handle);

    // Map dimensions.
    int _width, _height;

    // The locations of both players.
    location _player_one;
    location _player_two;

    // Indicates whether or not each cell in the board is passable.
    std::vector<bool> _is_wall;

    friend std::ostream& operator<<(std::ostream& output, const zz::map::direction& direction);
  };

}