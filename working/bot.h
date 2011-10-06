/* 
 * File:   bot.h
 * Author: montana
 *
 * Created on February 8, 2010, 2:49 PM
 */

#ifndef _ZZ_BOT_H
#define	_ZZ_BOT_H

#include "map.h"
#include "util.h"

#include <map>
#include <list>

#define DEBUG

namespace zz {

  class bot {

  public:
    bot( );
    virtual ~bot( );

    void set_map(FILE * file);
    void move( );
  private:

    bool running_script( );
    void set_script( int steps[], int num_steps );

    // Sends the bots move to the contest engine.
    void move_default( std::map<zz::map::direction, int> directions_minimax_scores );
    void move_random( std::map<zz::map::direction, int> directions_minimax_scores );
    void move_closer( std::map<zz::map::direction, int> directions_minimax_scores, zz::map::location destination = zz::map::location(-1,-1) );
    void move_first( std::map<zz::map::direction, int> directions_minimax_scores );
    void move_to_wall( std::map<zz::map::direction, int> directions_minimax_scores );
    void move_voronoi( std::map<zz::map::direction, int> directions_minimax_scores );
    void output_move( zz::map::direction direction );

    int _min_iterations;
    std::list<int> _script;
    std::list<int>::iterator _scripted_move;
    zz::map _map, _apocalyptic, _big_room, _center, _divider, _empty_room, _joust, _keyhole, _medium_room, _playground, _quadrant, _ring, _toronto, _trix, _u;
  };

  void minimax( int mins[], int& max, const zz::map& map, int iterations );
}


#endif	/* _ZZ_BOT_H */

