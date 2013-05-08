#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h> 
#include <pthread.h>
#include "config.h"
#include "matrix_definition.h"
#include "matrix.h"
#include "read_matrix.h"
#include "write_matrix.h"
#include "shared_matrix.h"
#include "metrics.h"
#include "error_handling.h"

typedef struct {
	Matrix *m1, *m2, *result;
	int start, end;
	pthread_t current;
} Input;

int** initMatrix(int rows, int columns) {
	int** matrix = (int**) calloc(rows, sizeof(int*));
	int i;
	
	for(i = 0; i < rows; i++) {
		matrix[i] = (int*) calloc(columns, sizeof(int));
	}

	return matrix;
}

void wait_all(pthread_t* slots, int size) {
  int i = 0;

	for (i = 0; i < size; i++) {
  	pthread_join(slots[i], NULL);
	}
}

void* doWork(void* arg) {
	Input* in = (Input*)arg;
	multiplyRowsByMatrix(in->m1, in->m2, in->result, in->start, in->end);	
	pthread_exit(NULL);
}

pthread_t startWorker(Matrix* m1, Matrix* m2, Matrix* result, int start, int end) {
	int status;
	pthread_attr_t attr;
	Input* input = (Input*)malloc(sizeof(Input));
	
	input->m1 = m1;
	input->m2 = m2;
	input->result = result;
	input->start = start;
	input->end = end;

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	status = pthread_create(&input->current, NULL, doWork, (void*)input);

  if (status != 0) {
  	printf("ERROR; return code from pthread_create() is %d\n", status);
  	exit(EXIT_FAILURE);
  }

  pthread_attr_destroy(&attr);
		
	return input->current;
}

Matrix* balanceWork(Matrix* m1, Matrix* m2, int numThreads) {
	int i, amountOfWorkToEach = 0, workLeft = 0, aditionalWork = 0, start, end, lastStartIndex = 0;
	Matrix* result = (Matrix*) malloc(sizeof(Matrix));
	pthread_t* threads = (pthread_t*) calloc(numThreads, sizeof(pthread_t));
	
	result->data = initMatrix(m1->rows, m2->columns);
	result->rows = m1->rows;
	result->columns = m2->columns;

	amountOfWorkToEach = (m1->rows / numThreads);
	workLeft = m1->rows % numThreads;
	aditionalWork = (workLeft > 0) ? 1 : 0;
	workLeft--;
	
	for (i = 0; i < numThreads; i++) {
		start = lastStartIndex;
		end = start + amountOfWorkToEach + aditionalWork;
		lastStartIndex = end;
		threads[i] = startWorker(m1, m2, result, start, end);
		aditionalWork = (workLeft > 0) ? 1 : 0;
		workLeft--;		
	}
	wait_all(threads, numThreads);
		
	return result;
}

int main(int argc, char** argv) {
	Matrix *m1 = NULL, *m2 = NULL, *resultSequential = NULL, *resultParallel = NULL;
	int numParallelTasks = 1;
	struct timeval diff;

	checkArgsSize(argc);
	
	m1 = readMatrixFromFile(M1);
	m2 = readMatrixFromFile(M2);
	 
	checkInputMatrices(m1, m2);
	checkNumTasks(argv, &numParallelTasks, m1);
		
	diff = measureBalanceWork(m1, m2, numParallelTasks, &resultParallel, balanceWork);
	writeMatrixInFile(OUT, resultParallel);
		 	
	printTime("Tempo levado para executar a multiplicação com %d threads: ", numParallelTasks, diff);

	pthread_exit(NULL);
}