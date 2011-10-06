/*
 * File:   MyTronBot.cc
 * Author: montana
 *
 * Created on February 8, 2010, 2:49 PM
 */

#include "bot.h"
#include <signal.h>
#include <sys/time.h>

namespace zz {

  class timer {
  public:
    static void time_move(){
      __bot.set_map( stdin );
      itimerval timer = { { 0, 0 }, { 0, 990000 } };
      setitimer(ITIMER_REAL, &timer, NULL);
      __bot.move( );
      timer.it_value.tv_usec = 0;
      setitimer(ITIMER_REAL, &timer, NULL);
    };

    static void time_out_handler( int /*signal*/ ) {
      __bot.move_immediately();
    }

  private:
    static zz::bot __bot;
  };

}

zz::bot zz::timer::__bot;


int main( ) {
  signal(SIGALRM, zz::timer::time_out_handler);
  while( true ) {
    zz::timer::time_move();
  }

  return 0;
}
