#include <iostream>

#include "benchmark.h"
#include "string.h"

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

    void minimax( int depth, bool calc_leafs = true ) {
      zz::map map("./maps/empty-room.txt");

      map.minimax( depth, calc_leafs );
      clock_t timer;
      timer = clock();
      map.minimax( depth, calc_leafs );
      timer = clock() - timer;
      std::cout << "minimax(" << depth << ", " << calc_leafs <<  ") time:" << timer << std::endl;

      map.minimax_new( depth, calc_leafs );
      timer = clock();
      map.minimax_new( depth, calc_leafs );
      timer = clock() - timer;
      std::cout << "minimax_new(" << depth << ", " << calc_leafs <<  ") time:" << timer << std::endl;
    }

    void solomax( int depth, bool calc_leafs = true) {
      zz::map map("./maps/empty-room.txt");

      map.solomax( depth, calc_leafs );
      clock_t timer;
      timer = clock();
      map.solomax( depth, calc_leafs );
      timer = clock() - timer;
      std::cout << "solomax(" << depth << ", " << calc_leafs <<  ") time:" << timer << std::endl;

      map.solomax_new( depth, calc_leafs );
      timer = clock();
      map.solomax_new( depth, calc_leafs );
      timer = clock() - timer;
      std::cout << "solomax_new(" << depth << ", " << calc_leafs << ") time:" << timer << std::endl;
    }

    void voronoi_territory( const char * filename ) {
      zz::map map(filename);
      clock_t timer;

      map.territory();
      map.territory();
      map.territory();
      map.territory();
      map.territory();
      int t;
      timer = clock();
      t = map.territory();
      timer = clock() - timer;
      std::cout << "voronoi territory: " << t << "  " << timer << std::endl;

//      map.voronoi_territory_new();
//      map.voronoi_territory_new();
//      map.voronoi_territory_new();
//      map.voronoi_territory_new();
//      map.voronoi_territory_new();
//      timer = clock();
//      t = map.voronoi_territory_new();
//      timer = clock() - timer;
//      std::cout << "voronoi territory new: " << t << "  " << timer << std::endl;
    }
  }
}

int main ()
{
//  benchmark_array_init();
//  benchmark_distance_from();
  for( int i = 0; i < 8; ++i ) {
    zz::benchmark::minimax(i);
    zz::benchmark::minimax(i,false);
    zz::benchmark::solomax(i);
    zz::benchmark::solomax(i,false);
  }
//  zz::benchmark::voronoi_territory( "./maps/empty-room.txt");
//  zz::benchmark::voronoi_territory( "./maps/apocalyptic.txt");
//  zz::benchmark::voronoi_territory( "./maps/divider.txt");
// zz::benchmark::benchmark_minimax_traversal();
//  zz::benchmark::benchmark_solomax();

  return 0;
}

