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

typedef struct {
	Matrix *m1, *m2, *result;
	int start, end, threadId;
} Input;

void wait_all(int* slots, int size) {
  int i = 0;

	for (i = 0; i < size; i++) {		
    wait(slots[i]);
    //printf("Parent (PID %d) waiting for child %d with PID %d to finish.\n", getpid(), i, slots[i]);
	}
}

void* doWork(void* arg) {
	Input* in = (Input*)arg;
	
	multiplyRowsByMatrix(in->m1, in->m2, in->result, in->start, in->end);	
	
	pthread_exit(0);
}

pthread_t start(Matrix* m1, Matrix* m2, Matrix* result, int start, int end, int threadId) {
	pthread_t thread;
	int status;
	Input* input = (Input*)malloc(sizeof(Input));
	
	input->m1 = m1;
	input->m2 = m2;
	input->result = result;
	input->start = start;
	input->end = end;
	input->threadId = threadId;
	
	status = pthread_create(&thread, NULL, doWork, (void*)input);
  if (status != 0)
  	exit(EXIT_FAILURE);
  
	//multiplyRowsByMatrix(m1, m2, result, start, end);	
	
	return thread;
}

Matrix* balanceWork(Matrix* m1, Matrix* m2, int numThreads) {
	int i, amountOfWorkToEach = 0, workLeft = 0, aditionalWork = 0, start, end, lastStartIndex = 0;
	Matrix* result = (Matrix*) malloc(sizeof(Matrix));
	pthread_t* threads = (pthread_t*) calloc(numThreads, sizeof(pthread_t));
	pid_t parent = getpid();
	thread current = parent;
	
	result->data = (int**) calloc(rows, sizeof(int*));;
	result->rows = rows;
	result->columns = columns;  
	
	amountOfWorkToEach = (m1->rows / numThreads);
	workLeft = m1->rows % numThreads;
	aditionalWork = (workLeft > 0) ? 1 : 0;
	workLeft--;
	
	for (i = 0; i < numThreads; i++) {
		if (current > 0) {
			start = lastStartIndex;
			end = start + amountOfWorkToEach + aditionalWork;
			lastStartIndex = end;
			current = start(m1, m2, result, start, end);
		}
		
		if (current > 0) {			
			threads[i] = current;
			aditionalWork = (workLeft > 0) ? 1 : 0;
			workLeft--;
		}
	}
	if (getpid() == parent)
		wait_all(threads, numThreads);		
		
	return result;
}

int main(int argc, char** argv) {
	Matrix *m1 = NULL, *m2 = NULL, *resultSequential = NULL, *resultParallel = NULL;
	int numProcesses = 1;
	struct timeval t1, t2, t3, t4, diff1, diff2;

	if (argc != 2) {
		printf("É necessário passar o parâmetro da quantidade de processos.\n");
		exit(EXIT_FAILURE);
	}
	
	m1 = readMatrixFromFile(M1);
	m2 = readMatrixFromFile(M2);
	 
	if (m1->columns != m2->rows) {
		printf("A matriz 1 tem quantidade de colunas diferente da quantidade de linhas da matriz 2, portanto não é possível efetuar a multiplicação delas.\n");
		exit(EXIT_FAILURE);
	}
	
	sscanf(argv[1], "%d", &numProcesses);
	if (m1->rows < numProcesses) {
		printf("O número de processos/threads não pode ser maior que a quantidade de linhas da primeira matriz de entrada.\n");
		exit(EXIT_FAILURE);
	}	
	
	gettimeofday(&t1, NULL);
	resultSequential = multiplyMatrices(m1, m2);
	gettimeofday(&t2, NULL);
	writeMatrixInFile(OUT, resultSequential);	
	
	gettimeofday(&t3, NULL);
	resultParallel = balanceWork(m1, m2, numProcesses);
	gettimeofday(&t4, NULL);
	//writeMatrixInOutput(resultParallel);
	writeMatrixInFile("output/outpar.txt", resultParallel);	
	
	timeval_subtract(&diff1, &t2, &t1);
	timeval_subtract(&diff2, &t4, &t3);  
 	printf("Tempo levado pela abordagem sequencial: ");
 	printTime(diff1);
 	printf("Tempo levado pela abordagem paralela: ");
	printTime(diff2);
	 
	return 0;
}
