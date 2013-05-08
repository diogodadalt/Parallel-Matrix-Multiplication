#include <sys/time.h> 
#include <stdio.h>
#include "metrics.h"

int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1) {
	long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
  result->tv_sec = diff / 1000000;
  result->tv_usec = diff % 1000000;

  return (diff<0);
}

void printTime(char* msg, int numTasks, struct timeval time) {
	printf(msg, numTasks);
	printf("%ld.%06ld\n", time.tv_sec, time.tv_usec);
}

struct timeval measureMultiplyMatrices(Matrix* m1, Matrix* m2, Matrix** result, Matrix* (*functionPtr)(Matrix*, Matrix*)) {
	struct timeval t1, t2, diff;
	gettimeofday(&t1, NULL);
	(*result) = functionPtr(m1, m2);
	gettimeofday(&t2, NULL);
	timeval_subtract(&diff, &t2, &t1);
	return diff;
}

struct timeval measureBalanceWork(Matrix* m1, Matrix* m2, int numTasks, Matrix** result, Matrix* (*functionPtr)(Matrix*, Matrix*, int)) {
	struct timeval t1, t2, diff;
	gettimeofday(&t1, NULL);
	(*result) = functionPtr(m1, m2, numTasks);
	gettimeofday(&t2, NULL);
	timeval_subtract(&diff, &t2, &t1);
	return diff;
}