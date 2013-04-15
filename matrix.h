#ifndef MATRIX_MODEL_H
#define MATRIX_MODEL_H

typedef struct {
	int rows, columns;
	int** data;
} Matrix;

int* getColumn(Matrix* m, int index);
int multiplyVectors(int* line, int* column, int size);
int* multiplyRowByMatrix(int* row, Matrix* m);
Matrix* multiplyMatrices(Matrix* m1, Matrix* m2);

#endif
