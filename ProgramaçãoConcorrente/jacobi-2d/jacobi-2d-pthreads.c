#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <papi.h>

typedef struct {
  float **matrix1, **matrix2;
  int size, start, end;
} mArgs;

float **initMatrix(int size) {
  int i, j;

  float **matrix = (float**) malloc(sizeof(float *) * size);
  for (i = 0; i < size; i++) 
    matrix[i] = (float *) malloc(sizeof(float) * size);

  float aux = 0;
  for (i = 0; i < size; i++) 
    for (j = 0; j < size; j++) {
      matrix[i][j] = aux;
      aux++;
    }
  
  return matrix;
}

void printMatrix(float **matrix, int size) {
  int i, j;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) 
      printf("%.2f", matrix[i][j]);

    printf("\n");
  }
}

//Parallel variables 
pthread_mutex_t barrierMutex;
pthread_cond_t releaseCondition;

//Used for unlock the barrier
int threadCounter = 0;

//Defined in command line (args)
int numThreads;

void threadBarrier() {
  pthread_mutex_lock(&barrierMutex);
  threadCounter++;

  if (threadCounter == numThreads) {
    pthread_cond_broadcast(&releaseCondition);
    threadCounter = 0;
  }
  else 
    pthread_cond_wait(&releaseCondition, &barrierMutex);

  pthread_mutex_unlock(&barrierMutex);
}

void *jacobi_2d_parallel(void *args) {
  
}

int main(int argv, char **argc) {
  int matrixSize = atoi(argc[1]);
  int nSteps = atoi(argc[2]);
  numThreads = atoi(argc[3]);

  float **matrix1 = initMatrix(matrixSize);
  float **matrix2 = initMatrix(matrixSize);

  free(matrix1);
  free(matrix2);

  return 0;
}
