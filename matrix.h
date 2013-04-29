#ifndef MATRIX_FUNCTIONS_H
#define MATRIX_FUNCTIONS_H

#include "matrix_definition.h"

int* getColumn(Matrix* m, int index);
int multiplyVectors(int* line, int* column, int size);
int* multiplyRowByMatrix(int* row, Matrix* m);
void multiplyRowByMatrix2(int* row, Matrix* m, int* finalRow);
Matrix* multiplyMatrices(Matrix* m1, Matrix* m2);
void multiplyRowsByMatrix(Matrix* m1, Matrix* m2, Matrix* result, int start, int end);

#endif
