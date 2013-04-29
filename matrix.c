#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>

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

int* multiplyRowByMatrix(int* row, Matrix* m) {
	int columns = m->columns;
	int* finalRow = (int*) calloc(columns, sizeof(int));
	int i =0;
	
	for (i = 0; i < columns; i++) {
		finalRow[i] = multiplyVectors(row, getColumn(m, i), m->rows);
	}
	
	return finalRow;
}

void multiplyRowByMatrix2(int* row, Matrix* m, int* finalRow) {
	int columns = m->columns;
	int i =0;
	
	for (i = 0; i < columns; i++) {
		finalRow[i] = multiplyVectors(row, getColumn(m, i), m->rows);
	}
}

Matrix* multiplyMatrices(Matrix* m1, Matrix* m2) {
	Matrix* result = (Matrix*) malloc(sizeof(Matrix));
	int rows = m1->rows;
	int columns = m2->columns;
	int** matrixData = (int**) calloc(rows, sizeof(int*));
	int i = 0, j = 0;
	
	for (i = 0; i < m1->rows; i++) {
		matrixData[i] = multiplyRowByMatrix(m1->data[i], m2);
	}
	
	result->rows = rows;
	result->columns = columns;
	result->data = matrixData;
	
	return result;
}

void multiplyRowsByMatrix(Matrix* m1, Matrix* m2, Matrix* result, int start, int end) {
	int i;
	//printf("start: %d, end: %d\n", start, end);
	for (i = start; i < end; i++) {
		//printf("\nprocess PID (%d) row: %d\n\n", getpid(), i);
		multiplyRowByMatrix2(m1->data[i], m2, result->data[i]);
		//printf("\nprocess PID (%d) row: %d\n\n", getpid(), i);
	}
	//writeMatrixInOutput(result);
}
