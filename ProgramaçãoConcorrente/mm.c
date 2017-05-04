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
	int aux = 0;

	for (j = 0; j < height; j++) {
		matrix[j] = (int *) malloc(width * sizeof(int));
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			//matrix[i][j] = rand() % 1000;
			matrix[i][j] = aux;
			aux++;
		}	
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
	int i, j, k;
	
  pthread_t *thread = malloc(sizeof(pthread_t *) * numThreads);

  matrixArgs *mArgs = malloc(sizeof(matrixArgs) * numThreads);
  for (k = 0; k < numThreads; k++) {
    mArgs[k].matrix1 = matrix1;
    mArgs[k].matrix2 = matrix2;
    mArgs[k].size = width1;
  }

	for (i = 0; i < height1; i++) {
		for (j = 0; j < width2; j++) {
      int aux = j;

			for (k = 0; k < numThreads; k++) {
				if (aux >= width2)
					break;

        //printf("%i %i\n", i, aux);

				pthread_create(&thread[k], NULL, parMulti, &mArgs[k]);
        aux++;
			}

      //printf("\n\n");

      aux = j;
			for (k = 0; k < numThreads; k++) {
				if (aux >=width2)
					break;
        printf("%i\n", mArgs[k].result);
				pthread_join(thread[k], NULL);
				resultMatrix[i][mArgs[k].j] = mArgs[k].result;
        aux++;
			}

      j+= numThreads;
		}
	}

	free(thread);
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
  resultMatrix = parMultiplyMatrix(matrix1, matrix2, matrixHeight1, matrixWidth1, matrixWidth2, nThreads);

	//printMatrix(resultMatrix, matrixWidth1, matrixHeight2);
}

