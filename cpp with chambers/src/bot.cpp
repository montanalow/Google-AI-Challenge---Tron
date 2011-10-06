/* 
 * File:   Bot.cpp
 * Author: montana
 * 
 * Created on February 8, 2010, 2:49 PM
 */

#include "bot.h"
#include "config.h"

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <algorithm>


zz::bot::bot( )
: _depth( 3 ), _players_are_connected( true ) {
  srand( time( NULL ) );
}

zz::bot::~bot( ) {
}

void zz::bot::set_map( FILE * file ) {
  _map.load_from_file( file );
}

void zz::bot::set_depth() {
  // figure out how long it takes to calculate the heuristic value of a leaf, by timing a
  // minimax run and counting the leafs at that depth, since 99% of a minimax
  // run is actually spent calculating the heuristic value of the leafs
  clock_t timer = 0;
  int test_depth = 2;
  zz::map::__minimax_leafs = 1;
  while( timer < CLOCKS_PER_SEC / 1000 ) {
    // if minimax is faster than 1ms then we'll need to test until we do enough work to actually get a real time
    timer = clock( );
    minimax( test_depth );
    timer = clock( ) - timer;
    #ifdef DEBUG_BOT
      fprintf( stderr, " test_depth: %d | leafs: %8d | %-.4fs\n", test_depth, zz::map::__minimax_leafs, (float) timer / CLOCKS_PER_SEC );
    #endif
    if( zz::map::__minimax_leafs == 0 ) {
      // minimax solved the board using test_depth
      _depth = test_depth;
      return;
    } else {
      ++test_depth;
    }
  }

  // calculate the max number of leafs we can handle in 1 second
  int leafs = zz::map::__minimax_leafs;
  float clocks_per_leaf = ( float ) timer / leafs;
  int max_leafs = (CLOCKS_PER_SEC / clocks_per_leaf) * 0.90; // give up 10% for overhead, and to play it safe

  // find the max minimax depth, with fewer than max_leafs
  bool gone_over = false;
  int cost = 0;
  do {
    // see how many leafs are at the current depth
    timer = clock( );
    minimax( _depth, false );
    cost = ( clock( ) - timer ) / clocks_per_leaf; // we burned some time
    max_leafs -= cost;
    leafs = zz::map::__minimax_leafs;
#ifdef DEBUG_BOT
  fprintf( stderr, " leafs: %8d | max_leafs: %8d | depth: %2d | %-.4fs | cost: %d\n", leafs, max_leafs, _depth, (float) ( clock( ) - timer ) / CLOCKS_PER_SEC, leafs + cost * 4 );
#endif

    // test deeper, if we have enough max leafs left for double the cost that this depth took
    if( leafs + cost * 2 < max_leafs ) {
      _depth++;
    }
    if( leafs > max_leafs ) {
      _depth--;
      gone_over = true;
    }
  } while( leafs > 0 && max_leafs > 0 && !gone_over && leafs + cost * 2 < max_leafs );
}

void zz::bot::minimax( int depth, bool use_heuristic ){
  zz::map::__minimax_leafs = 0;
  if( _players_are_connected ) {
    _map.minimax( depth, use_heuristic );
  } else {
    _map.solomax( depth, use_heuristic );
  }
}

void zz::bot::move( ) {
#ifdef DEBUG_BOT
  fprintf( stderr, "(%d,%d) vs (%d,%d) calculating move:\n", _map.player_one().x(), _map.player_one().y(), _map.player_two().x(), _map.player_two().y() );
#endif
  // determins the basic strategy of minimax vs solomax
  if( _players_are_connected ) {
    _players_are_connected = _map.locations_are_connected( _map.player_one( ), _map.player_two( ) );
    if( !_players_are_connected )
      _depth = 3; // we are gonna switch from minimax to solomax, solomax can't alpha-beta prune, so we need to start low again
  }

  set_depth();

  // find the best possible hueristic solution to the board using _depth
  clock_t timer = clock( );
  minimax( _depth );
  timer = clock( ) - timer;

  // build a list of the best moves
  int count_moves = 0;
  int moves[4];
  for( int i = 0; i < 4; ++i){
    if( _map.mins()[i] == _map.max() ){
      moves[count_moves++] = i;
    }
  }

#ifdef DEBUG_BOT
  fprintf( stderr, " %s(%d): [", _players_are_connected ? "minimax" : "solomax", _depth );
  for( int i = 0; i < 4; ++i ) {
    fprintf( stderr, "%d:%d", i, _map.mins()[i] );
    if( i !=  3) fprintf( stderr, ", " );
  }
  fprintf( stderr, "] (%d) %-.4fs\n", _map.max(), (float) timer / CLOCKS_PER_SEC );
#endif

  choose_greedily( moves, count_moves );
}

void zz::bot::choose_first( int moves[], int count_moves ) {
  output_move( moves[0] );
}

void zz::bot::choose_randomly( int moves[], int count_moves ) {
  int index = rand( ) % count_moves;
#ifdef DEBUG_BOT
    fprintf( stderr, " choose_randomly: [" );
    for( int i = 0; i < count_moves; ++i ) {
      fprintf( stderr, "%d:%d", moves[i], i == index );
      if( i != count_moves - 1) fprintf( stderr, ", " );
    }
    fprintf( stderr, "]\n" );
#endif
  output_move( moves[index] );
}

void zz::bot::choose_greedily( int moves[], int count_moves ) {
  if( count_moves == 1 ) {
    // don't bother if there is only 1 move
    choose_first( moves, count_moves );
  } else {
    clock_t timer = clock( );
    // use minimax( 0 ) to check the immediate value of each move
    _map.minimax( 0 );

    int max = INT_MIN;  // find the max greedy move in moves
    for( int i = 0; i < count_moves; ++i ){
      if( _map.mins()[moves[i]] > max ) {
        max = _map.mins()[moves[i]];
      }
    }

    // collect all moves that have the max immediate value
    int greedy_moves[count_moves];
    int count_greedy_moves = 0;
    for( int i = 0; i < count_moves; ++i ){
      if( _map.mins()[moves[i]] == max ) {
        greedy_moves[count_greedy_moves++] = moves[i];
      }
    }

#ifdef DEBUG_BOT
    fprintf( stderr, " choose_greedily: [" );
    for( int i = 0; i < count_moves; ++i ) {
      fprintf( stderr, "%d:%d", moves[i], _map.mins()[moves[i]] );
      if( i != count_moves - 1) fprintf( stderr, ", " );
    }
    fprintf( stderr, "] (%d) %-.4fs\n", max, ( float ) ( clock( ) - timer ) / CLOCKS_PER_SEC );
#endif

    if( count_greedy_moves > 1 ) {
      choose_by_walls( greedy_moves, count_greedy_moves );
    } else {
      choose_first( greedy_moves, count_greedy_moves );
    }
  }
}


void zz::bot::choose_by_walls( int moves[], int count_moves ) {
  if( count_moves == 1 ) {
    // don't bother if there is only 1 move
    choose_first( moves, count_moves );
  } else {
    clock_t timer = clock( );
    // calc the number of walls next to each square
    int moves_walls[count_moves];
    for( int i = 0; i < count_moves; ++i ) {
      moves_walls[i] = 0;
      zz::map::location next_spot = _map.player_one( ).adjacent( moves[i] );
      for( int j = 0; j < 4; ++j ) {
        if( _map.is_wall( next_spot.adjacent( j ) ) ) {
          ++moves_walls[i];
        }
      }
    }

    // find the moves with the most walls
    int max_walls = *std::max_element( moves_walls, moves_walls + count_moves );
    int max_wall_moves[count_moves];
    int count_max_wall_moves = 0;
    for( int i = 0; i < count_moves; ++i ){
      if( moves_walls[i] == max_walls ) {
        max_wall_moves[count_max_wall_moves++] = moves[i];
      }
    }

  #ifdef DEBUG_BOT
    fprintf( stderr, " choose_by_walls: [" );
    for( int i = 0; i < count_moves; ++i ) {
      fprintf( stderr, "%d:%d", moves[i], moves_walls[i] );
      if( i != count_moves - 1) fprintf( stderr, ", " );
    }
    fprintf( stderr, "] (%d) %-.4fs\n", max_walls, ( float ) ( clock( ) - timer ) / CLOCKS_PER_SEC );
  #endif

    if( count_max_wall_moves > 1 ){
      choose_randomly( max_wall_moves, count_max_wall_moves );
    } else {
      choose_first( max_wall_moves, count_max_wall_moves );
    }
  }
}

void zz::bot::choose_by_distance( int moves[], int count_moves ) {
  if( count_moves == 1 ) {
    // don't bother if there is only 1 move
    choose_first( moves, count_moves );
  } else {
    clock_t timer = clock( );
    // provide a default destination
    zz::map::location destination = _map.player_two( );

    // find the distance for all moves from the destination
    int moves_distances[count_moves];
    for( int i = 0; i < count_moves; ++i ) {
      zz::map::location next_spot = _map.player_one( ).adjacent( moves[i] );
      moves_distances[i] = pow( next_spot.x( ) - destination.x( ), 2 ) + pow( next_spot.y( ) - destination.y( ), 2 );
    }

    // collect all moves that have the minimum distance
    int min_distance = *std::min_element( moves_distances, moves_distances + count_moves);
    int closer_moves[count_moves];
    int count_closer_moves = 0;
    for( int i = 0; i < count_moves; ++i ){
      if( moves_distances[i] == min_distance ) {
        closer_moves[count_closer_moves++] = moves[i];
      }
    }

#ifdef DEBUG_BOT
    fprintf( stderr, " choose_by_distance: [" );
    for( int i = 0; i < count_moves; ++i ) {
      fprintf( stderr, "%d:%d", moves[i], moves_distances[i] );
      if( i != count_moves - 1) fprintf( stderr, ", " );
    }
    fprintf( stderr, "] (%d) %-.4fs\n", min_distance, ( float ) ( clock( ) - timer ) / CLOCKS_PER_SEC );
#endif
    
    if( count_closer_moves > 1 ) {
      choose_by_walls( closer_moves, count_closer_moves );
    } else {
      choose_first( closer_moves, count_closer_moves );
    }
  }
}

void zz::bot::choose_by_territory( int moves[], int count_moves ) {
  if( count_moves == 1 ) {
    // don't bother if there is only 1 move
    choose_first( moves, count_moves );
  } else {
    clock_t timer = clock( );
    // find the distance for all moves from the destination
    int moves_territory[count_moves];
    zz::map::location player_one = _map.player_one();
    for( int i = 0; i < count_moves; ++i ) {
      zz::map::location next_spot = _map.player_one( ).adjacent( moves[i] );
      _map.set_player_one( next_spot );
      moves_territory[i] = _map.territory();
      _map.set_player_one( player_one );
      _map.set_is_wall( next_spot, false );
    }

    // collect all moves that have the minimum distance
    int max_territory = *std::max_element( moves_territory, moves_territory + count_moves);
    int territorial_moves[count_moves];
    int count_territorial_moves = 0;
    for( int i = 0; i < count_moves; ++i ){
      if( moves_territory[i] == max_territory ) {
        territorial_moves[count_territorial_moves++] = moves[i];
      }
    }

#ifdef DEBUG_BOT
    fprintf( stderr, " choose_by_territory: [" );
    for( int i = 0; i < count_moves; ++i ) {
      fprintf( stderr, "%d:%d", moves[i], moves_territory[i] );
      if( i != count_moves - 1) fprintf( stderr, ", " );
    }
    fprintf( stderr, "] (%d) %-.4fs\n", max_territory, ( float ) ( clock( ) - timer ) / CLOCKS_PER_SEC );
#endif

    if( count_territorial_moves > 1 ) {
      choose_by_walls( territorial_moves, count_territorial_moves );
    } else {
      choose_first( territorial_moves, count_territorial_moves );
    }
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
#ifdef DEBUG_BOT
      fprintf( stderr, "Moving North!\n" );
#endif
      move = 1;
      break;
    case zz::map::east :
#ifdef DEBUG_BOT
      fprintf( stderr, "Moving East!\n" );
#endif
      move = 2;
      break;
    case zz::map::south :
#ifdef DEBUG_BOT
      fprintf( stderr, "Moving South!\n" );
#endif
      move = 3;
      break;
    case zz::map::west :
#ifdef DEBUG_BOT
      fprintf( stderr, "Moving West!\n" );
#endif
      move = 4;
      break;
  }

  fprintf( stdout, "%d\n", move );
  fflush( stdout );
}
