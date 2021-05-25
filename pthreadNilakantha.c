#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define BILLION 1E9
#define NUM_THREADS 20
#define t 10000000000

double parcialPi[NUM_THREADS];

double piNila(long long k)
{
  long long max = t * (k + 1) / NUM_THREADS;
  long long min = t * k / NUM_THREADS;
  double x = 0.0;
  int sig = (min + 2) % 4 == 2 ? -1 : 1;
  long long i;
  for (i = min + 2; i <= max; i += 2)
  {
    sig *= -1;
    x += sig * 4.0 / (i) / (i + 1) / (i + 2);
  }
  return x;
}

void *piCal(void *i)
{
  int k;
  k = *((int *)i);
  parcialPi[k] = piNila(k);
  return NULL;
}

double plusPi()
{
  int i;
  double pi = 0.0;
  for (i = 0; i < NUM_THREADS; i++)
  {
    pi += parcialPi[i];
  }
  return 3.0 + pi;
}

void main()
{
  printf("Calculate Pi on Pthread (Nilakantha Series)\n");

  // initialize
  pthread_t threads[NUM_THREADS];
  int thread;
  int i;

  // create
  for (i = 0; i < NUM_THREADS; i++)
  {
    thread = pthread_create(&threads[i], NULL, piCal, &i);
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
  for (i = 0; i < NUM_THREADS; i++)
  {
    void *status;
    pthread_join(threads[i], &status);
  }

  // end time
  clock_gettime(CLOCK_MONOTONIC, &end);
  double duration = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION;

  // outcome
  printf("Pi is %.30lf\n", plusPi());
  printf("Time in secs : %lfs\n", duration);
}