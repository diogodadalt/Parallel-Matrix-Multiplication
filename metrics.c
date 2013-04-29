#include <sys/time.h> 
#include <stdio.h>

int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1) {
	long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
  result->tv_sec = diff / 1000000;
  result->tv_usec = diff % 1000000;

  return (diff<0);
}

void printTime(struct timeval time) {
	printf("%ld.%06ld\n", time.tv_sec, time.tv_usec);
}
