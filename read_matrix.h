#ifndef READ_MATRIX_H
#define READ_MATRIX_H

#include "matrix.h"

char* trimwhitespace(char *str);
int* readVectorFromString(char* line, int columns);
Matrix* readMatrixFromFile(char* filePath);

#endif
