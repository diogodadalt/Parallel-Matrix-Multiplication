#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include "matrix_definition.h"

void checkArgsSize(int argSize);
void checkInputMatrices(Matrix* m1, Matrix* m2);
void checkNumTasks(char** argv, int* numTasks, Matrix* m1);

#endif