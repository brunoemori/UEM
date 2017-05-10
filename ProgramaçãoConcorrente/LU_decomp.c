#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int **createMatrix(int height, int width) {
	int **matrix = (int **) malloc(sizeof(int*) * height);
	int i;
	for (i = 0; i < height; i++) 
		matrix[i] = (int *) malloc(sizeof(int) * height);

	return matrix;
}

void initMatrix(int **matrix, int height, int width) {
	int i, j, aux = 0;
	for (i = 0; i < height; i++) 
		for (j = 0; j < width; j++) { 
			matrix[i][j] = aux;
			aux++;
		}
}

void initLowerTriangularMatrix(int **matrix, int height, int width) {
	int i, j, aux = 1;
	for (i = 0; i < height; i++) 
		for (j = 0; j < width; j++) {
			if (i < j)
				matrix[i][j] = 0;
			else {
				matrix[i][j] = aux;
				aux++;
			}
		}
}

void initUpperTriangularMatrix(int **matrix, int height, int width) {
	int i, j, aux = 1;
	for (i = 0; i < height; i++) 
		for (j = 0; j < width; j++) {
			if (i > j)
				matrix[i][j] = 0;
			else {
				matrix[i][j] = aux;
				aux++;
			}
		}
}

void printMatrix(int **matrix, int height, int width) {
	int i, j;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) 
			printf("%i ", matrix[i][j]);
		printf("\n");
	}
}

void sequential(int **matrix, int **result1, int **result2) {
	
}

int main(int argv, char **argc) {
	int height = atoi(argc[1]);
	int width = height;
	int numThreads;

	int **matrix = createMatrix(height, width);
	initMatrix(matrix, height, width);
	//printMatrix(matrix, height, width);

	int **result1 = createMatrix(height, width);
	initLowerTriangularMatrix(result1, height, width);
	//printMatrix(result1, height, width);

	int **result2 = createMatrix(height, width);
	initUpperTriangularMatrix(result2, height, width);
	printMatrix(result2, height, width);

	
}
