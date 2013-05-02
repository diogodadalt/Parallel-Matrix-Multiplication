#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include <sys/time.h> 
#include "config.h"
#include "matrix_definition.h"
#include "matrix.h"
#include "read_matrix.h"
#include "write_matrix.h"
#include "shared_matrix.h"
#include "metrics.h"
#include "error_handling.h"

void wait_all(int* slots, int size) {
  int i = 0;

	for (i = 0; i < size; i++) {		
    waitpid(slots[i], 0, 0);
    //printf("Parent (PID %d) waiting for child %d with PID %d to finish.\n", getpid(), i, slots[i]);
	}
}

pid_t doWork(Matrix* m1, Matrix* m2, Matrix* result, int start, int end) {
	pid_t pid = fork();
	if (pid == 0) {
		//printf("DoWork child with (PID %d).\n", getpid());
		Matrix* result = readSharedMatrix(m1->rows, m2->columns);
		//writeMatrixInConsole(result);
		multiplyRowsByMatrix(m1, m2, result, start, end);
		//detach(result);
		free(result);
		exit(EXIT_SUCCESS);
	}
	return pid;
}

Matrix* balanceWork(Matrix* m1, Matrix* m2, int numParallelTasks) {
	int i, amountOfWorkToEach = 0, workLeft = 0, aditionalWork = 0, start, end, lastStartIndex = 0;
	Matrix* result = createSharedMatrix(m1->rows, m2->columns);
	pid_t* pids = (pid_t*) calloc(numParallelTasks, sizeof(pid_t));
	pid_t parent = getpid();
	pid_t current = parent;
	
	amountOfWorkToEach = (m1->rows / numParallelTasks);
	workLeft = m1->rows % numParallelTasks;
	aditionalWork = (workLeft > 0) ? 1 : 0;
	workLeft--;
	
	for (i = 0; i < numParallelTasks; i++) {
		if (current > 0) {
			start = lastStartIndex;
			end = start + amountOfWorkToEach + aditionalWork;
			lastStartIndex = end;
			current = doWork(m1, m2, result, start, end);
		}
		
		if (current > 0) {
			pids[i] = current;
			aditionalWork = (workLeft > 0) ? 1 : 0;
			workLeft--;
		}
	}
	if (getpid() == parent)
		wait_all(pids, numParallelTasks);		
		
	return result;
}

int main(int argc, char** argv) {
	Matrix *m1 = NULL, *m2 = NULL, *resultSequential = NULL, *resultParallel = NULL;
	int numParallelTasks = 1;
	struct timeval diff1, diff2;

	checkArgsSize(argc);
	
	m1 = readMatrixFromFile(M1);
	m2 = readMatrixFromFile(M2);
	 
	checkInputMatrices(m1, m2);
	checkNumTasks(argv, &numParallelTasks, m1);
		
	diff1 = measureMultiplyMatrices(m1, m2, &resultSequential, multiplyMatrices);
	writeMatrixInFile(OUT, resultSequential);	
	
	diff2 = measureBalanceWork(m1, m2, numParallelTasks, &resultParallel, balanceWork);	
	writeMatrixInFile("output/outparthreads.txt", resultParallel);	
	
	printTime(diff1, "Tempo levado pela abordagem sequencial: ");
	printTime(diff2, "Tempo levado pela abordagem paralela: ");

	return 0;
}