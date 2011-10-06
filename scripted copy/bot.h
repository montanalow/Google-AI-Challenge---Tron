/* 
 * File:   Bot.h
 * Author: montana
 *
 * Created on February 8, 2010, 2:49 PM
 */

#ifndef _BOT_H
#define	_BOT_H

#include "map.h"

namespace zz {

  class bot {
  public:
    bot();
    virtual ~bot();

    void move();

  private:
    zz::map _map;

    // returns the bots best move
    zz::map::direction calculate_move();
    
    // Sends the bots move to the contest engine.
    void output_move( zz::map::direction direction );
  };

}

#endif	/* _BOT_H */

