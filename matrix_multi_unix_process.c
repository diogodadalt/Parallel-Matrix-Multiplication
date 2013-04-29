#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h> 
#include "config.h"
#include "matrix_definition.h"
#include "matrix.h"
#include "read_matrix.h"
#include "write_matrix.h"
#include "shared_matrix.h"
#include "metrics.h"

void wait_all(int* slots, int size) {
  int i = 0;

	for (i = 0; i < size; i++) {		
    wait(slots[i]);
    //printf("Parent (PID %d) waiting for child %d with PID %d to finish.\n", getpid(), i, slots[i]);
	}
}

pid_t doWork(Matrix* m1, Matrix* m2, Matrix* result, int start, int end) {
	pid_t pid = fork();
	if (pid == 0) {
		//printf("DoWork child with (PID %d).\n", getpid());
		Matrix* result = readSharedMatrix(m1->rows, m2->columns);
		//writeMatrixInOutput(result);
		multiplyRowsByMatrix(m1, m2, result, start, end);
		//detach(result);
		free(result);
		exit(0);
	}
	return pid;
}

Matrix* balanceWork(Matrix* m1, Matrix* m2, int numProcesses) {
	int i, amountOfWorkToEach = 0, workLeft = 0, aditionalWork = 0, start, end, lastStartIndex = 0;
	Matrix* result = createSharedMatrix(m1->rows, m2->columns);
	pid_t* pids = (pid_t*) calloc(numProcesses, sizeof(pid_t));
	pid_t parent = getpid();
	pid_t current = parent;
	
	amountOfWorkToEach = (m1->rows / numProcesses);
	workLeft = m1->rows % numProcesses;
	aditionalWork = (workLeft > 0) ? 1 : 0;
	workLeft--;
	
	for (i = 0; i < numProcesses; i++) {
		if (current > 0) {
			start = lastStartIndex;
			end = start + amountOfWorkToEach + aditionalWork;
			lastStartIndex = end;
			current = doWork(m1, m2, result, start, end);
		}
		
		if (current > 0) {
			//printf("balanceWork parent with (PID %d), start: %d, end: %d, amountOfWorkToEach: %d, aditionalWork: %d.\n", getpid(), i, i + amountOfWorkToEach + aditionalWork, amountOfWorkToEach, aditionalWork);
			pids[i] = current;
			aditionalWork = (workLeft > 0) ? 1 : 0;
			workLeft--;
		}
	}
	if (getpid() == parent)
		wait_all(pids, numProcesses);		
		
	return result;
}

int main(int argc, char** argv) {
	Matrix *m1 = NULL, *m2 = NULL, *result = NULL;
	int numProcesses = 1;
	pid_t* pids;
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
	result = multiplyMatrices(m1, m2);
	gettimeofday(&t2, NULL);
	writeMatrixInFile(OUT, result);	
	
	gettimeofday(&t3, NULL);
	Matrix* parResult = balanceWork(m1, m2, numProcesses);
	gettimeofday(&t4, NULL);
	//writeMatrixInOutput(parResult);
	writeMatrixInFile("output/outpar.txt", parResult);	
	
	timeval_subtract(&diff1, &t2, &t1);
	timeval_subtract(&diff2, &t4, &t3);  
 	printf("Tempo levado pela abordagem sequencial: ");
 	printTime(diff1);
 	printf("Tempo levado pela abordagem paralela: ");
	printTime(diff2);
	 
	return 0;
}
