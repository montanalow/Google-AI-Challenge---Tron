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

    void set_map( FILE * file );
    void move( );

  private:
    void set_depth( );
    void minimax( int depth, bool use_heuristic = true );

    // Chooses a move from the valid ones returned from minimax
    void choose_first( );
    void choose_by_walls( );
//    void choose_by_expected_value( );
    void choose_randomly();
    void choose_greedily();
    //    void choose_by_distance();
    //    void choose_by_territory();

    // Sends the bots move to the contest engine.
    void output_move( zz::map::direction direction );

    zz::map _map;
    int _depth;
    bool _players_are_connected;
    int _count_moves;
    int _moves[4];
    bool _moved;
  };
}


#endif	/* _ZZ_BOT_H */

