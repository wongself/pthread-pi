#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define BILLION 1E9
#define NUM_THREADS 20
#define ITERATIONS 20000000000

void *piCal(void *threadid)
{
  // initialize
  long id;
  id = (long)threadid;
  int tid = (int)id;
  int r = time(NULL) * (tid + 1);

  long long in_count = 0;
  long long total_iterations = ITERATIONS / NUM_THREADS;
  long long counter = 0;
  long long slice = total_iterations / 10;

  // calculate
  for (counter = 0; counter < total_iterations; counter++)
  {
    if (tid == 0 && counter % slice == 0)
    {
      printf("Thread 0: %d0%%\n", (int)(counter / slice));
    }

    double x = (double)rand_r(&r) / (double)RAND_MAX;
    double y = (double)rand_r(&r) / (double)RAND_MAX;
    x = x * 2 - 1;
    y = y * 2 - 1;
    double result = x * x + y * y;
    if (result <= 1)
    {
      in_count = in_count + 1; //check if the generated value is inside a unit circle
    }
  }

  // get the remaining iterations calculated by thread 0
  if (tid == 0)
  {
    long long remainder = ITERATIONS % NUM_THREADS;
    for (counter = 0; counter < remainder; counter++)
    {
      double x = (double)rand_r(&r) / (double)RAND_MAX;
      double y = (double)rand_r(&r) / (double)RAND_MAX;
      x = x * 2 - 1;
      y = y * 2 - 1;

      double result = x * x + y * y;

      if (result <= 1)
      {
        in_count = in_count + 1; // check if the generated value is inside a unit circle
      }
    }
  }
  pthread_exit((void *)in_count);
}

int main(int argc, char *argv)
{
  printf("Calculate Pi on Pthread (Monte Carlo)\n");

  // initialize
  pthread_t threads[NUM_THREADS];
  int thread;
  long t;
  long threads_in = 0;

  // create
  for (t = 0; t < NUM_THREADS; t++)
  {
    thread = pthread_create(&threads[t], NULL, piCal, (void *)t);
    if (thread)
    {
      printf("ERROR; return code from pthread_create() is %d\n", thread);
      exit(-1);
    }
  }

  // start time
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  // join
  for (t = 0; t < NUM_THREADS; t++)
  {
    void *status;
    pthread_join(threads[t], &status);
    // keep track of the total in count
    long x = (long)status;
    threads_in = threads_in + x;
  }

  // end time
  clock_gettime(CLOCK_MONOTONIC, &end);
  double duration = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION;

  // outcome
  printf("Point in circle %ld\n", threads_in);
  printf("Point not in circle %ld\n", ITERATIONS);
  printf("Pi is %.20lf\n", 4 * ((double)threads_in / (double)ITERATIONS));
  printf("Time in secs : %lfs\n", duration);

  pthread_exit(NULL);
}
