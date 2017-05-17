/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/* jacobi-2d.c: this file is part of PolyBench/C */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

/* Include polybench common header. */
#include <polybench.h>

/* Include benchmark-specific header. */
#include "jacobi-2d-pthreads.h"

//Struct args
typedef struct {
  int start, end;
} fArgs;

DATA_TYPE **A, **B;

static void initMatrix(int n) {
  int i, j;
  A = (DATA_TYPE **) malloc(sizeof(DATA_TYPE *) * n);
  for (i = 0; i < n; i++) 
    A[i] = (DATA_TYPE *) malloc(sizeof(DATA_TYPE) * n);


  B = (DATA_TYPE **) malloc(sizeof(DATA_TYPE *) * n);
  for (i = 0; i < n; i++) 
    B[i] = (DATA_TYPE *) malloc(sizeof(DATA_TYPE) * n);

  for (i = 0; i < n; i++) 
    for (j = 0; j < n; j++) {
      A[i][j] = ((DATA_TYPE) i * (j + 2) + 2) / n;
      B[i][j] = ((DATA_TYPE) i * (j + 3) + 3) / n;
    }
}

/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static void kernel_jacobi_2d(int tsteps, int n) {
  int t, i, j;

#pragma scop
  for (t = 0; t < _PB_TSTEPS; t++) {
    for (i = 1; i < _PB_N - 1; i++)
	    for (j = 1; j < _PB_N - 1; j++)
	      B[i][j] = SCALAR_VAL(0.2) * (A[i][j] + A[i][j-1] + A[i][1+j] + A[1+i][j] + A[i-1][j]);

    for (i = 1; i < _PB_N - 1; i++)
	    for (j = 1; j < _PB_N - 1; j++)
	      A[i][j] = SCALAR_VAL(0.2) * (B[i][j] + B[i][j-1] + B[i][1+j] + B[1+i][j] + B[i-1][j]);
    }
#pragma endscop
}

//Parallel -------------------------------------------------
pthread_mutex_t barrierMutex;
pthread_cond_t releaseCondition;
int threadsArrived = 0;
int numThreads; //Passed as command line (args)

void threadBarrier() {
  pthread_mutex_lock(&barrierMutex);
  threadsArrived++;

  if (threadsArrived == numThreads) {
    threadsArrived = 0;
    pthread_cond_broadcast(&releaseCondition);
  }

  else
    pthread_cond_wait(&releaseCondition, &barrierMutex);

  pthread_mutex_unlock(&barrierMutex);
}

void kernel_jacobi_2d_parallel(void *args) {
    
}

int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int n = N;
  int tsteps = TSTEPS;
  //int numThreads = atoi(argv[1]);
  int i;

  initMatrix(n);

  //Defining threads & arguments
  /*
  pthread_t *threads = malloc(sizeof(pthread_t *) * numThreads);
  fArgs *jacobiArgs = malloc(sizeof(fArgs *) * numThreads);

  int linesPerThread = (n - 2) / numThreads;
  int rest = n % numThreads;

  for (i = 0; i < numThreads; i++) {
    jacobiArgs[i].start = 1;
    jacobiArgs[i].end = linesPerThread;
  }

  for (i = 0; i < numThreads; i++) {
    jacobiArgs[i].start++;
    rest--;

    if (rest == 0)
      break;
  }

  for (i = 1; i < numThreads; i++) {
    jacobiArgs[i].start = jacobiArgs[i - 1].end + 1;
    jacobiArgs[i].end = jacobiArgs[i].start + linesPerThread - 1;
  }
  */

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_jacobi_2d(tsteps, n);
  /*
  for (i = 0; i < numThreads; i++) {
    pthread_create(&threads[i], NULL, kernel_jacobi_2d_parallel, &args); 
  }
  */

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(B);

  return 0;
}
