/* 
 * File:   benchmark.h
 * Author: montana
 *
 * Created on February 13, 2010, 11:28 AM
 */

#ifndef _BENCHMARK_H
#define	_BENCHMARK_H

#define BENCHMARK

#include "map.h"
#include "bot.h"

namespace zz {
  namespace benchmark {
    static clock_t bot_move = 0;
    static clock_t bot_minimax = 0;
  }
}

#endif	/* _BENCHMARK_H */
