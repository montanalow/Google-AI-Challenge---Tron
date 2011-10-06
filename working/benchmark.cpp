#include <iostream>

#include "benchmark.h"

#define BENCHMARK

namespace zz {
  namespace benchmark {
    clock_t move = 0;
    clock_t map_minimax = 0;
    clock_t map_area_available_from = 0;
    clock_t map_distance_between = 0;

    void benchmark_array_init () {
      clock_t timer;
      int a[1000000];
      timer = clock();
      memset( a, 0, 1000000 * sizeof(int));
      timer = clock() - timer;
      int sum = 0;
      for( int i = 0; i < 1000000; ++i ){
        sum += a[i];
      }
      std::cout << "memset: (" << sum << ") " << timer << std::endl;

      timer = clock();
      int *b = a;
      while( b < a + 1000000) { (*b++) = 0; }
      timer = clock() - timer;
      sum = 0;
      for( int i = 0; i < 1000000; ++i ){
        sum += a[i];
      }
      std::cout << "while: (" << sum << ") " << timer << std::endl;

      timer = clock();
      for( int i = 0; i < 1000000; ++i ) { a[i] = 0; }
      timer = clock() - timer;
      sum = 0;
      for( int i = 0; i < 1000000; ++i ){
        sum += a[i];
      }
      std::cout << "for: (" << sum << ") " << timer << std::endl;
    }

    void benchmark_distance_from() {
      clock_t timer;
      zz::map map("./maps/empty-room.txt");
      int d = 0;
      int sum = 0;
      timer = clock();
      for( int i = 0; i < map.width(); ++i ){
        for( int j = 0; j < map.height(); ++j){
          for( int k = 0; k < map.width(); ++k ){
            for( int l = 0; l < map.height(); ++l){
              d = map.distance_between( zz::map::location(i,j), zz::map::location(k,l));
              sum += d;
            }
          }
        }
      }
      timer = clock() - timer;
      std::cout << "distance_from: (" << sum << ") " << timer << std::endl;
    }

    void benchmark_move() {
    //  zz::bot bot;
    //
    //
    //  FILE* f = fopen( "./maps/huge-room.txt", "r" );
    //  if( f ) {
    //    bot.set_map( f );
    //    fclose( f );
    //  }
    //
    //  zz::benchmark::bot_move = 0;
    //  zz::benchmark::bot_minimax = 0;
    //  bot.move();

    }

    void minimax() {
      zz::map map("./maps/trix.txt");

      clock_t timer;
      int max;
      int mins[4];
      timer = clock();
      max = map.minimax( 2 );
      timer = clock() - timer;
      std::cout << "map::minimax : " << timer << std::endl;
      std::cout << "map::map_area_available_from : " << map_area_available_from << std::endl;
      std::cout << max << std::endl;
//      timer = clock();
//      max = map.minimax( 6 );
//      timer = clock() - timer;
//      std::cout << "map::minimax :" << timer << std::endl;
//      timer = clock();
//      max = map.minimax( 7 );
//      timer = clock() - timer;
//      std::cout << "map::minimax :" << timer << std::endl;
//
//      timer = clock();
//      zz::minimax( mins, max, map, 5 );
//      timer = clock() - timer;
//      std::cout << "map::minimax :" << timer << std::endl;
//
//      timer = clock();
//      max = map.minimax( 5 );
//      timer = clock() - timer;
//      std::cout << "map::minimax :" << timer << std::endl;
//
//      timer = clock();
//      zz::minimax( mins, max, map, 5 );
//      timer = clock() - timer;
//      std::cout << "minimax : " << timer << std::endl;

//      zz::benchmark::map_minimax = 0;
//      zz::benchmark::map_area_available_from = 0;
//      int max = map.minimax( 3 );
//      std::cout << "map::minimax : " << zz::benchmark::map_minimax << std::endl;
//      std::cout << "map::area_available_from : " << zz::benchmark::map_area_available_from << std::endl;
//
//      zz::benchmark::map_minimax = 0;
//      zz::benchmark::map_area_available_from = 0;
//      max = map.minimax( 4 );
//      std::cout << "map::minimax : " << zz::benchmark::map_minimax << std::endl;
//      std::cout << "map::area_available_from : " << zz::benchmark::map_area_available_from << std::endl;
//
//      zz::benchmark::map_minimax = 0;
//      zz::benchmark::map_area_available_from = 0;
//      max = map.minimax( 5 );
//      std::cout << "map::minimax : " << zz::benchmark::map_minimax << std::endl;
//      std::cout << "map::area_available_from : " << zz::benchmark::map_area_available_from << std::endl;
    }
  }
}

int main ()
{
//  benchmark_array_init();
//  benchmark_distance_from();
  zz::benchmark::minimax();

  return 0;
}

