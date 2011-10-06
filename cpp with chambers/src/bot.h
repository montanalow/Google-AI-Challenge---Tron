/* 
 * File:   bot.h
 * Author: montana
 *
 * Created on February 8, 2010, 2:49 PM
 */

#ifndef _ZZ_BOT_H
#define	_ZZ_BOT_H

#include "map.h"
#include <stdio.h>

namespace zz {
  class bot {

  public:
    typedef int script;
    
    bot( );
    virtual ~bot( );

    void set_map(FILE * file);
    void move( );

  private:
    void set_depth();
    void minimax( int depth, bool use_heuristic = true );

    // Chooses a move from the valid ones returned from minimax
    void choose_first( int moves[], int count_moves );
    void choose_randomly( int moves[], int count_moves );
    void choose_greedily( int moves[], int count_moves );
    void choose_by_walls( int moves[], int count_moves );
    void choose_by_distance( int moves[], int count_moves );
    void choose_by_territory( int moves[], int count_moves );

    // Sends the bots move to the contest engine.
    void output_move( zz::map::direction direction );

    zz::map _map;
    int _depth;
    bool _players_are_connected;
  };
}


#endif	/* _ZZ_BOT_H */

