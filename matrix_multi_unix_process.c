#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "matrix.h"
#include "read_matrix.h"
#include "write_matrix.h"

int* getColumn(Matrix* m, int index) {
	int rows = m->rows;
	int** matrixData = m->data;
	int* column = (int*) calloc(rows, sizeof(int));
	int i = 0;

	for (i = 0; i < rows; i++)
		column[i] = matrixData[i][index];
  
  return column;
}

int multiplyVectors(int* line, int* column, int size) {
	int cellValue = 0, i = 0;
	
	for (i = 0; i < size; i++)
		cellValue += line[i] * column[i];
	
	return cellValue;
}

Matrix* multiplyMatrices(Matrix* m1, Matrix* m2) {
	Matrix* result = (Matrix*) malloc(sizeof(Matrix));
	int rows = m1->rows;
	int columns = m2->columns;
	int** matrixData = (int**) calloc(rows, sizeof(int*));
	int i = 0, j = 0;
	
	for (i = 0; i < m1->rows; i++) {
		matrixData[i] = (int*) calloc(columns, sizeof(int));
		for (j = 0; j < m2->columns; j++) {
			matrixData[i][j] = multiplyVectors(m1->data[i], getColumn(m2, j), rows);
		}
	}
	
	result->rows = rows;
	result->columns = columns;
	result->data = matrixData;
	
	return result;
}

int main(int argc, char** argv) {

	Matrix *m1 = NULL, *m2 = NULL, *result = NULL;

	if (argc != 2) {
		printf("É necessário passar o parâmetro da quantidade de processos.\n");
		exit(EXIT_FAILURE);
	}
	
	m1 = readMatrixFromFile(M1);
	m2 = readMatrixFromFile(M2);
	 
	if (m1->columns != m2->rows) {
		printf("A matriz 1 tem quantidade de colunas diferente da quantidade de linhas da matriz 2, portanto não é possível efetuar a multiplicação delas.\n");
		exit(EXIT_FAILURE);
	}
	
	result = multiplyMatrices(m1, m2);	
	writeMatrixInFile(OUT, result);
	 
	return 0;
}
