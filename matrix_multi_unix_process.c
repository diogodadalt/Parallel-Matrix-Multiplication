#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "matrix.h"
#include "read_matrix.h"
#include "write_matrix.h"

void wait_all(int* slots, int size) {
	int i = 0;

	for (i = 0; i < size; i++) {		
		wait(slots[i]);
		printf("Parent (PID %d) waiting for child %d with PID %d to finish.\n", getpid(), i, slots[i]);
	}
}

void startProcesses(pid_t* slots, int numProcesses, int current) {
	pid_t pid;
	if (numProcesses > 0 && numProcesses >= (current + 1)) {
		pid = fork();
		if (pid > 0) {
			slots[current] = pid;
			printf("I'm the parent (PID %d) spawning the child (number %d) with (PID %d)\n", getpid(), current, pid);
			startProcesses(slots, numProcesses, current+1);
			if (numProcesses == (current + 1)) {
				wait_all(slots, numProcesses);
			}
		} else if (pid == 0) {
			printf("I am a child: %d PID: %d\n", current, getpid());
			//sleep(2);
			return;
		}
	}
}

int main(int argc, char** argv) {

	Matrix *m1 = NULL, *m2 = NULL, *result = NULL;
	int numProcesses = 1;
	pid_t* pids;

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
	
	result = multiplyMatrices(m1, m2);	
	writeMatrixInFile(OUT, result);
		
	sscanf(argv[1], "%d", &numProcesses);
	pids = (pid_t*) calloc(numProcesses, sizeof(pid_t));		
	startProcesses(pids, numProcesses, 0);
	 
	return 0;
}
