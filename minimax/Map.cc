// Map.cc

#include "Map.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>
#include <set>


const zz::map::direction zz::map::north = 0;
const zz::map::direction zz::map::east = 1;
const zz::map::direction zz::map::west = 2;
const zz::map::direction zz::map::south = 3;

zz::map::map( )
: _width( -1 ), _height( -1 ), _player_one( -1, -1 ), _player_two( -1, -1 ), _is_wall( ) {
  read_from_file( stdin );
}

zz::map::map( const zz::map& other )
: _width( other._width ), _height( other._height ), _player_one( other._player_one ), _player_two( other._player_two ), _is_wall( other._is_wall ) {
}

std::vector<zz::map::direction> zz::map::get_open_directions_for( zz::map::location location ) {
  std::vector<direction> open_directions;

  if (!is_wall( location.first, location.second - 1 )) {
    open_directions.push_back( north );
  }
  if (!is_wall( location.first + 1, location.second )) {
    open_directions.push_back( east );
  }
  if (!is_wall( location.first, location.second + 1 )) {
    open_directions.push_back( south );
  }
  if (!is_wall( location.first - 1, location.second )) {
    open_directions.push_back( west );
  }
  return open_directions;
}

int zz::map::width( ) const {
  return _width;
}

int zz::map::height( ) const {
  return _height;
}

bool zz::map::is_wall( const map::location& location ) const {
  return is_wall( location.first, location.second );
}

bool zz::map::is_wall( int x, int y ) const {
  if (x < 0 || y < 0 || x >= _width || y >= _height) {
    return true;
  } else {
    return _is_wall[x + _width * y];
  }
}

void zz::map::read_from_file( FILE *file_handle ) {
  int x, y, c;
  int num_items = fscanf( file_handle, "%d %d\n", &_width, &_height );
  if (feof( file_handle ) || num_items < 2) {
    exit( 0 ); // End of stream means end of game. Just exit.
  }
  _is_wall = std::vector<bool>(_width * _height);

  x = 0;
  y = 0;
  while (y < _height && (c = fgetc( file_handle )) != EOF) {
    switch (c) {
      case '\r':
        break;
      case '\n':
        if (x != _width) {
          fprintf( stderr, "x != width in Board_ReadFromStream\n" );
          return;
        }
        ++y;
        x = 0;
        break;
      case '#':
        if (x >= _width) {
          fprintf( stderr, "x >= width in Board_ReadFromStream\n" );
          return;
        }
        _is_wall[x + _width * y] = true;
        ++x;
        break;
      case ' ':
        if (x >= _width) {
          fprintf( stderr, "x >= width in Board_ReadFromStream\n" );
          return;
        }
        _is_wall[x + _width * y] = false;
        ++x;
        break;
      case '1':
        if (x >= _width) {
          fprintf( stderr, "x >= width in Board_ReadFromStream\n" );
          return;
        }
        _is_wall[x + _width * y] = true;
        _player_one = map::location( x, y );
        ++x;
        break;
      case '2':
        if (x >= _width) {
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

void zz::map::make_move( ) {
  std::map<zz::map::direction, float> area_scores;
  for (int i = 0; i < 4; ++i) {
    zz::map::location location = move_from( _player_one, i );
    area_scores[i] = area_available_from( location );
  }
  int max_area = 0;
  for (int i = 0; i < 4; ++i) {
    if (area_scores[i] > max_area) max_area = area_scores[i];
  }

  std::cerr << "area_scores: [";
  for (int i = 0; i < 4; ++i) {
    std::cerr << (int) i << ":" << (float) area_scores[i] << ", ";
  }
  std::cerr << "] (" << max_area << ")" << std::endl;

  std::vector<zz::map::direction> better_moves;
  for (int i = 0; i < 4; ++i) {
    if (area_scores[i] == max_area) {
      better_moves.push_back( i );
    }
  }

  std::cerr << "better_moves: [";
  for (int i = 0; i < better_moves.size( ); ++i) {
    std::cerr << (float) better_moves[i] << ", ";
  }
  std::cerr << "]" << std::endl;

  if (better_moves.size( ) > 1) {
    std::map<zz::map::direction, std::pair<float, int> > move_scores = evaluate_moves( );
    float max_score = -1;
    for (int i = 0; i < 4; ++i) {
      if (move_scores[i].first > max_score && area_scores[i] == max_area) max_score = move_scores[i].first;
    }
    std::cerr << "move_scores: [";
    for (int i = 0; i < 4; ++i) {
      std::cerr << (int) i << ":" << (float) move_scores[i].first << "|" << (float) move_scores[i].second << ", ";
    }
    std::cerr << "] (" << max_score << ")" << std::endl;

    std::vector<zz::map::direction> good_moves;
    for (int i = 0; i < 4; ++i) {
      if (move_scores[i].first == max_score && area_scores[i] == max_area) {
        good_moves.push_back( i );
      }
    }
    std::cerr << "good_moves: [";
    for (int i = 0; i < good_moves.size( ); ++i) {
      std::cerr << (float) good_moves[i] << ", ";
    }
    std::cerr << "]" << std::endl;

    if (good_moves.size( ) > 1) {
      // differentiate good moves by the amount of open area (joined areas will not be differentiated)
      std::vector<zz::map::direction> best_moves;
      int max_branches = 0;
      for (int i = 0; i < 4; ++i) {
        if (move_scores[i].second > max_branches && move_scores[i].first == max_score && area_scores[i] == max_area) max_branches = move_scores[i].second;
      }
      for (int i = 0; i < 4; ++i) {
        if (move_scores[i].first == max_score && area_scores[i] == max_area && move_scores[i].second == max_branches) {
          best_moves.push_back( i );
        }
      }
      std::cerr << "best_moves: [";
      for (int i = 0; i < best_moves.size( ); ++i) {
        std::cerr << (float) best_moves[i] << ", ";
      }
      std::cerr << "]" << std::endl;
      make_move( best_moves.front( ) );
    } else {
      make_move( good_moves.front( ) );
    }
  } else if (better_moves.size( ) == 1) {
    make_move( better_moves.front( ) );
  } else {
    make_move( north );
  }
}

void zz::map::make_move( zz::map::direction direction ) {
  int move = 1;
  switch( direction ) {
    case north:
      move = 1;
      break;
    case east:
      move = 2;
      break;
    case south:
      move = 3;
      break;
    case west:
      move = 4;
      break;
  }
  std::cerr << "MAKING MOVE: " << (int) move << std::endl;
  fprintf( stdout, "%d\n", move ); // my directions are 0 indexed, theirs are 1 indexed
  fflush( stdout );
}

std::map<zz::map::direction, std::pair<float, int> > zz::map::evaluate_moves( int depth ) {
  bool debug = false;

  // find the worst thing that could happen for each of my moves, so we can prioritize the least bad moves
  float player_one_moves[4] = { -1, -1, -1, -1 };
  for (int i = 0; i < 4; ++i) {
    float min = 1;
    for (int j = 0; j < 4; ++j) {
      if (moves[i][j] < min) min = moves[i][j];
    }
    player_one_moves[i] = min;
  }
  // find the best thing that could happen for each of my opponents moves, so we can assume those are the moves he takes
  float player_two_moves[4] = { 1, 1, 1, 1 };
  for (int j = 0; j < 4; ++j) {
    float max = -1;
    for (int i = 0; i < 4; ++i) {
      if (moves[i][j] > max) max = moves[i][j];
    }
    player_two_moves[j] = max;
  }

  // find the max possible move value this turn for each player
  float player_one_max = -1;
  for (int i = 0; i < 4; ++i) {
    if (player_one_moves[i] > player_one_max) player_one_max = player_one_moves[i];
  }
  float player_two_max = 1;
  for (int i = 0; i < 4; ++i) {
    if (player_two_moves[i] < player_two_max) player_two_max = player_two_moves[i];
  }

  // find the highest priority set of moves for each player
  bool player_one_priority_moves[4] = { false, false, false, false };
  int priority_moves_count = 0;
  for (int i = 0; i < 4; ++i) {
    if (player_one_moves[i] == player_one_max) {
      player_one_priority_moves[i] = true;
      ++priority_moves_count;
    }
  }
  bool player_two_priority_moves[4] = { false, false, false, false };
  for (int i = 0; i < 4; ++i) {
    if (player_two_moves[i] == player_two_max) {
      player_two_priority_moves[i] = true;
    }
  }

  std::map<zz::map::direction, std::pair<float, int> > move_scores;
  for (int i = 0; i < 4; ++i) {
    move_scores[i] = std::pair<float, int>(player_one_moves[i], priority_moves_count);
  }

  if (debug) {
    for (int i = 0; i < depth; ++i) {
      std::cerr << "  "; // indent
    }
    std::cerr << "(" << (int) _player_one.first << "," << (int) _player_one.second << ") vs (" << (int) _player_two.first << "," << (int) _player_two.second << ") scores: [";
    for (int i = 0; i < 4; ++i) {
      std::cerr << (int) i << ":" << (float) move_scores[i].first << ", ";
    }
    std::cerr << "] (" << player_one_max << ")" << std::endl;
  }


  if (depth < 7 && player_one_max == 0) { // look ahead one more turn if another turn is possible
    int next_branches[4] = { 0, 0, 0, 0 };
    float next_min_maxes[4] = { 2, 2, 2, 2 };
    for (int i = 0; i < 4; ++i) {
      if (player_one_priority_moves[i]) {
        float next_maxes[4] = { -2, -2, -2, -2 };
        for (int j = 0; j < 4; ++j) {
          if (player_two_priority_moves[j]) {
            // only consider future moves, that both people would consider highest priority
            if (debug) {
              for (int k = 0; k < depth; ++k) {
                std::cerr << "  "; // indent
              }
              std::cerr << "move: " << (int) i << " : " << (int) j << std::endl;
            }

            zz::map map( *this );
            map.set_player_one( map.move_from( map._player_one, i ) );
            map.set_player_two( map.move_from( map._player_two, j ) );
            std::map<zz::map::direction, std::pair<float, int> > next_move_scores = map.evaluate_moves( depth + 1 );

            for (int k = 0; k < 4; ++k) {
              if (next_move_scores[k].first > next_maxes[j]) {
                next_maxes[j] = next_move_scores[k].first;
              }
              next_branches[i] += next_move_scores[k].second;
            }
          }
        }
        for (int j = 0; j < 4; ++j) {
          if (next_maxes[j] > -2 && next_maxes[j] < next_min_maxes[i]) {
            next_min_maxes[i] = next_maxes[j];
          }
        }
      }
    }

    if (debug) {
      for (int i = 0; i < depth; ++i) {
        std::cerr << "  "; // indent
      }
      std::cerr << "next_min_maxes: [";
      for (int i = 0; i < 4; ++i) {
        std::cerr << (float) next_min_maxes[i] << ", ";
      }
      std::cerr << "]" << std::endl;
    }

    for (int i = 0; i < 4; ++i) {
      if (next_min_maxes[i] != 2) {
        move_scores[i].first = next_min_maxes[i]; // move_scores[i].first + (next_min_maxes[i] / 1.1);
        move_scores[i].second += next_branches[i];
      }
    }
  }

  if (debug) {
    for (int i = 0; i < depth; ++i) {
      std::cerr << "  "; // indent
    }
    std::cerr << "(" << (int) _player_one.first << "," << (int) _player_one.second << ") vs (" << (int) _player_two.first << "," << (int) _player_two.second << ") after scores: [";
    for (int i = 0; i < 4; ++i) {
      std::cerr << (int) i << ":" << (float) move_scores[i].first << ", ";
    }
    std::cerr << "]" << std::endl;
  }

  return move_scores;
}

int zz::map::area_available_from( zz::map::location location, std::set<map::location>* counted_locations ) {
  bool cleanup = false;
  if (counted_locations == 0) {
    cleanup = true;
    counted_locations = new std::set<map::location > ();
  };

  if (is_wall( location )) return 0;

  int available_area = 1;
  std::vector<zz::map::direction> directions = get_open_directions_for( location );
  for (int i = 0; i < directions.size( ); ++i) {

    zz::map::direction move = directions[i];
    zz::map::location destination = move_from( location, move );
    if (counted_locations->count( destination ) == 0) {
      counted_locations->insert( destination );
      available_area += area_available_from( destination, counted_locations );
    }
  }

  if (cleanup) {
    delete counted_locations;
  }

  return available_area;
}

void zz::map::set_player_one( map::location location ) {

  _player_one = location;
  _is_wall[_player_one.first + _width * _player_one.second] = true;
}

void zz::map::set_player_two( map::location location ) {

  _player_two = location;
  _is_wall[_player_two.first + _width * _player_two.second] = true;
}

zz::map::location zz::map::move_from( const zz::map::location& location, const zz::map::direction direction ) {
  if (direction == north) {
    return map::location( location.first, location.second - 1 );
  } else if (direction == east) {
    return map::location( location.first + 1, location.second );
  } else if (direction == south) {
    return map::location( location.first, location.second + 1 );
  } else if (direction == west) {

    return map::location( location.first - 1, location.second );
  }
  return location;
}

std::ostream & operator<<(std::ostream& output, const zz::map::direction& direction) {
  if (direction == zz::map::north) {
    output << "N";
  } else if (direction == zz::map::east) {
    output << "E";
  } else if (direction == zz::map::south) {
    output << "S";
  } else if (direction == zz::map::west) {
    output << "W";
  }
  return output; // for multiple << operators.
}
