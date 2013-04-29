#ifndef MATRIX_MODEL_H
#define MATRIX_MODEL_H

#include "matrix_definition.h"

#define SHM_KEY 5678

Matrix* createSharedMatrix(int rows, int columns);
Matrix* readSharedMatrix(int rows, int columns);
void detach(Matrix* matrix);

#endif
