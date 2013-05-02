#ifndef METRICS_H
#define METRICS_H

#include "matrix_definition.h"

int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1);
void printTime(struct timeval time, char* msg);
struct timeval measureMultiplyMatrices(Matrix* m1, Matrix* m2, Matrix** result, Matrix* (*functionPtr)(Matrix*, Matrix*));
struct timeval measureBalanceWork(Matrix* m1, Matrix* m2, int numTasks, Matrix** result, Matrix* (*functionPtr)(Matrix*, Matrix*, int));

#endif
