#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

typedef struct {
  int **matrix1, **matrix2;
  int start;
  int matrixHeight1, matrixWidth1, matrixWidth2; 
  int *result;
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
	int aux = 0;

	for (j = 0; j < height; j++) {
		matrix[j] = (int *) malloc(width * sizeof(int));
	}
	return matrix;
}

void **initMatrix(int **matrix, int height, int width) {
  int i, j, aux = 0;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			//matrix[i][j] = rand() % 1000;
			matrix[i][j] = aux;
			aux++;
		}	
	}
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
  mArgs->result = createMatrix(mArgs->height, mArgs->width);
  int i, j, k;
  int i = mArgs->line;

  for (i = mArgs->start; i < mArgs->height1; i++) {
    for (j = 0; j < mArgs->width2; j++)  
      int aux = 0
        for (k = 0; k < width1; k++)
        aux += mArgs->matrix1[mArgs->line][k] * mArgs->matrix2[k][j];

    mArgs->result[i][j] = aux;
  }
}

int **parMultiplyMatrix(int **matrix1, int **matrix2, int height1, int width1, int width2, int numThreads) {
	int **resultMatrix = createMatrix(matrixHeight1, matrixWidth2);
	int i = 0, k, currentLine = 0;
	
  pthread_t *thread = malloc(sizeof(pthread_t *) * numThreads);

  matrixArgs *mArgs = malloc(sizeof(matrixArgs) * numThreads);

  int linesPerThread = height1 / numThreads;
  int rest = height % numThreads;

  mArgs[0].matrix1 = matrix1;
  mArgs[0].matrix2 = matrix2;
  mArgs[0].start = 0;
  mArgs[0].matrixHeight1 = linesPerThread;
  mArgs[0].matrixWidth1 = width1;
  mArgs[0].matrixWidth2 = width2;

  for (k = 1; k < numThreads; k++) {
    mArgs[k].start = mArgs[k - 1].start + linesPerThread - 1;
  }

  for (i = 0; i < height1; i++) {
  }

    for (k = i; k < threadsCreated; k++) {
      resultMatrix[i] = mArgs[k].result;
    }
  }
	free(thread);
  free(matrixArgs)
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

  initMatrix(matrix1, matrixHeight1, matrixWidth1);
  initMatrix(matrix2, matrixHeight2, matrixWidth2);

  /*
  printMatrix(matrix1, matrixHeight1, matrixWidth1);
  printMatrix(matrix2, matrixHeight2, matrixWidth2);
  */

  //resultMatrix = multiplyMatrix(matrix1, matrix2, matrixHeight1, matrixWidth1, matrixWidth2);
  //resultMatrix = parMultiplyMatrix(matrix1, matrix2, matrixHeight1, matrixWidth1, matrixWidth2, nThreads);

	//printMatrix(resultMatrix, matrixWidth1, matrixHeight2);
}

