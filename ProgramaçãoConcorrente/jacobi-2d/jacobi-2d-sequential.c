#include <stdio.h>
#include <stdlib.h>

double **initMatrix(int size) {
  int i, j;

  double **matrix = (double**) malloc(sizeof(double *) * size);
  for (i = 0; i < size; i++) 
    matrix[i] = (double *) malloc(sizeof(double) * size);

  return matrix;
}


void setData(double **matrix1, double **matrix2, int size) {
  int i, j;
  for (i = 0; i < size; i++) 
    for (j = 0; j < size; j++) {
      matrix1[i][j] = ((double) i * (j + 2) + 2) / size;
      matrix2[i][j] = ((double) i * (j + 3) + 3) / size;
    }
}

void printMatrix(double **matrix, int size) {
  int i, j;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) 
      printf("%.2f", matrix[i][j]);

    printf("\n");
  }
}

void jacobi_2d_sequential(double **matrix1, double **matrix2, int size, int steps) {
  int s, i, j;
  for (s = 0; s < steps; s++) {
    for (i = 1; i < size - 1; i++)
      for (j = 1; j < size - 1; j++)
        matrix2[i][j] = (double) 0.2 * (matrix1[i][j] + matrix1[i - 1][j] + matrix1[i + 1][j] + matrix1[i][j - 1] + matrix1[i][j + 1]);
  
  
    for (i = 1; i < size - 1; i++)
      for (j = 1; j < size - 1; j++)
        matrix1[i][j] = (double) 0.2 * (matrix2[i][j] + matrix2[i - 1][j] + matrix2[i + 1][j] + matrix2[i][j - 1] + matrix2[i][j + 1]);
  }
}

int main(int argv, char **argc) {
  int matrixSize = atoi(argc[1]);
  int nSteps = atoi(argc[2]);

  double **matrix1 = initMatrix(matrixSize);
  double **matrix2 = initMatrix(matrixSize);

  setData(matrix1, matrix2, matrixSize);

  jacobi_2d_sequential(matrix1, matrix2, matrixSize, nSteps);

  free(matrix1);
  free(matrix2);

  return 0;
}
