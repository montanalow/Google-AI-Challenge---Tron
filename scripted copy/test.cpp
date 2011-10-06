/* time example */
#include <stdio.h>
#include <time.h>

#include <vector>
int main ()
{
  printf ("%d\n", CLOCKS_PER_SEC );
  int sum = 0;
  clock_t start_cycles = clock();
  for(int i = 0; i < 100000; ++i ){
    std::vector<int> v(225);
    for( int j = 0; j < v.size(); ++j ){
      sum += v[j];
    }
  }
  clock_t end_cycles = clock();
  printf ("total: %d | stack vector %ld\n", sum, end_cycles - start_cycles);


  start_cycles = clock();
  for(int i = 0; i < 100000; ++i ){
    int * v = new int[225];
    for( int j = 0; j < 225; ++j ){
      v[j] = 0;
    }
    for( int j = 0; j < 225; ++j ){
      sum += (v[j]);
    }
    delete[] v;
  }
  end_cycles = clock();
  printf ("total: %d | heap assignment %ld\n", sum, end_cycles - start_cycles);

  start_cycles = clock();
  for(int i = 0; i < 100000; ++i ){
    int * v = new int[225];
    memset( v, 0, 225 * sizeof(int)); // initialize all values to 0
    for( int j = 0; j < 225; ++j ){
      sum += v[j];
    }
    delete[] v;
  }
  end_cycles = clock();
  printf ("total: %d | heap memset %ld\n", sum, end_cycles - start_cycles);

  start_cycles = clock();
  for(int i = 0; i < 100000; ++i ){
    int v[255];
    for( int j = 0; j < 225; ++j ){
      v[j] = 0;
    }
    for( int j = 0; j < 225; ++j ){
      sum += v[j];
    }
  }
  end_cycles = clock();
  printf ("total: %d | stack - set %ld\n", sum, end_cycles - start_cycles);


  start_cycles = clock();
  for(int i = 0; i < 100000; ++i ){
    int v[255];
    memset( &v, 0, 225 * sizeof(int)); // initialize all values to 0
    for( int j = 0; j < 225; ++j ){
      sum += v[j];
    }
  }
  end_cycles = clock();
  printf ("total: %d | stack - memset %ld\n", sum, end_cycles - start_cycles);


  start_cycles = clock();
  for(int i = 0; i < 100000; ++i ){
    std::vector<int> v(225);
    for( int j = 0; j < v.size(); ++j ){
      v[j] = 0;
    }
    for( int j = 0; j < 225; ++j ){
      sum += v[j];
    }
  }
  end_cycles = clock();
  printf ("total: %d | %ld\n", sum, end_cycles - start_cycles);

  start_cycles = clock();
  for(int i = 0; i < 100000; ++i ){
    int * v = new int[225];
    for( int j = 0; j < 225; ++j ){
      v[j] = 0;
    }
    for( int j = 0; j < 225; ++j ){
      sum += v[j];
    }
    delete[] v;
  }
  end_cycles = clock();
  printf ("total: %d | %ld\n", sum, end_cycles - start_cycles);

  start_cycles = clock();
  for(int i = 0; i < 100000; ++i ){
    int * v = new int[225];
    memset( v, 0, 225 * sizeof(int)); // initialize all values to 0
    for( int j = 0; j < 225; ++j ){
      sum += v[j];
    }
    delete[] v;
  }
  end_cycles = clock();
  printf ("total: %d | %ld\n", sum, end_cycles - start_cycles);

  start_cycles = clock();
  for(int i = 0; i < 100000; ++i ){
    int v[255];
    for( int j = 0; j < 225; ++j ){
      v[j] = 0;
    }
    for( int j = 0; j < 225; ++j ){
      sum += v[j];
    }
  }
  end_cycles = clock();
  printf ("total: %d | %ld\n", sum, end_cycles - start_cycles);


  return 0;
}