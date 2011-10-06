/* 
 * File:   benchmark.h
 * Author: montana
 *
 * Created on February 13, 2010, 11:28 AM
 */

#ifndef _BENCHMARK_H
#define	_BENCHMARK_H

#include "map.h"
#include "bot.h"

namespace zz {
  namespace benchmark {
    extern clock_t move;
    extern clock_t map_minimax;
    extern clock_t map_area_available_from;
    extern clock_t map_distance_between;
  }
}

#endif	/* _BENCHMARK_H */
