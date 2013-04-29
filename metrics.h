#ifndef METRICS_H
#define METRICS_H

int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1);
void printTime(struct timeval time);

#endif
