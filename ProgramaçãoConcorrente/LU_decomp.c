#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

float **createMatrix(int size) {
	float **matrix = (float **) malloc(sizeof(float*) * size);
	int i;
	for (i = 0; i < size; i++) 
		matrix[i] = (float *) malloc(sizeof(float) * size);

	return matrix;
}

void initMatrix(float **matrix, int size) {
	int i, j;
	float aux = 1;
	for (i = 0; i < size; i++) 
		for (j = 0; j < size; j++) { 
			matrix[i][j] = aux;
			aux++;
		}
}

void getLowerTriangularMatrix(float **matrix, float **result, int size) {
	int i, j;
	for (i = 0; i < size; i++) 
		for (j = 0; j < size; j++) {
			if (i < j)
				result[i][j] = 0;
			else {
				if (i == j)
					result[i][j] = 1;
				else 
					result[i][j] = matrix[i][j];
			}
		}
}

void getUpperTriangularMatrix(float **matrix, float **result, int size) {
	int i, j, aux = 1;
	for (i = 0; i < size; i++) 
		for (j = 0; j < size; j++) {
			if (i > j)
				result[i][j] = 0;
			else 
				result[i][j] = matrix[i][j];
		}
}

void printMatrix(float **matrix, int size) {
	int i, j;
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) 
			printf("%.2f ", matrix[i][j]);
		printf("\n");
	}
}

void sequential(float **matrix, int size) {
	int i, j, k;
	for (i = 1; i < size; i++) {
		for (j = i; j < size; j++) {
			for (k = 1; k < i - 1; k++) 
				matrix[i][j] -= (matrix[i][k] * matrix[k][j]);
		}
	
		for (j = i + 1; j < size; j++) {
			for (k = 1; k < i - 1; k++) 
				matrix[j][i] -= (matrix[j][k] * matrix[k][i]);
			
			matrix[j][i] /= matrix[i][i];
		}
	}
}

int main(int argv, char **argc) {
	int size = atoi(argc[1]);
	int numThreads;

	float **matrix = createMatrix(size);
	initMatrix(matrix, size);
	printMatrix(matrix, size);
	
	sequential(matrix, size);

	//result 1 = L
	float **result1 = createMatrix(size);
	getLowerTriangularMatrix(matrix, result1, size);

	//result2 = U
	float **result2 = createMatrix(size);
	getUpperTriangularMatrix(matrix, result2, size);

	printf("\n\n");
	printMatrix(result1, size);
	printf("\n\n");
	printMatrix(result2, size);

	
}
