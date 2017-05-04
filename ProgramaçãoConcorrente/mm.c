#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

typedef struct {
  int **matrix1, **matrix2;
  int i, j, size, result;
} matrixArgs;

void printMatrix(int **matrix, int height, int width) {
	int i, j;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) 
			printf("%i ", matrix[i][j]);
		printf("\n");
	}
}

int **createMatrix(int height, int width) {
	int **matrix = (int**) malloc(height * sizeof(int*));
	int i, j;

	for (j = 0; j < height; j++) {
		matrix[j] = (int *) malloc(width * sizeof(int));
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) 
			matrix[i][j] = rand() % 1000;
	}

	return matrix;
}

//Sequential
int **multiplyMatrix(int **matrix1, int **matrix2, int height1, int width1, int width2) {
	int **resultMatrix = createMatrix(height1, width2);
	int i, j, k;

	for (i = 0; i < height1; i++) {
		for (j = 0; j < width2; j++) {
			int aux = 0;
			for (k = 0; k < width1; k++) 
				aux += matrix1[i][k] * matrix2[k][j];
			resultMatrix[i][j] = aux;
		}
	}
	return resultMatrix;
}

//Parallel
void *parMulti(void *args) {
  matrixArgs *mArgs = args;
  int k, aux = 0;
  int i = mArgs->i;
  int j = mArgs->j;

  for (k = 0; k < mArgs->size; k++) 
    aux += mArgs->matrix1[i][k] * mArgs->matrix2[k][j];

  mArgs->result = aux;
}

int **parMultiplyMatrix(int **matrix1, int **matrix2, int height1, int width1, int width2, int numThreads) {
	int **resultMatrix = createMatrix(height1, width2);
	int i, j;
	
  pthread_t thread[2];
  int threadCount = 0;

  matrixArgs *mArgs = malloc(sizeof(matrixArgs));
  mArgs->matrix1 = matrix1;
  mArgs->matrix2 = matrix2;
  mArgs->size = width1;

	for (i = 0; i < height1; i++) {
		for (j = 0; j < width2; j++) {
      mArgs->i = i;
      mArgs->j = j;

      if (threadCount >= 2) 
        threadCount = 0;
    
      pthread_create(&thread[threadCount], NULL, parMulti, mArgs);
      pthread_join(thread[threadCount], NULL);
      threadCount++;
      resultMatrix[i][j] = mArgs->result;
		}
	}
	return resultMatrix;
}


int main(int argc, char** argv) {
	int matrixHeight1 = atoi(argv[1]);
	int matrixWidth1 = atoi(argv[2]);
	int matrixHeight2 = atoi(argv[3]);
	int matrixWidth2 = atoi(argv[4]);
	int nThreads = atoi(argv[5]);
	
	int	**matrix1, **matrix2, **resultMatrix;
	matrix1 = createMatrix(matrixHeight1, matrixWidth1);
	matrix2 = createMatrix(matrixHeight2, matrixWidth2);

	//resultMatrix = multiplyMatrix(matrix1, matrix2, matrixHeight1, matrixWidth1, matrixWidth2);

  resultMatrix = parMultiplyMatrix(matrix1, matrix2, matrixHeight1, matrixWidth1, matrixWidth2, 1);
}
