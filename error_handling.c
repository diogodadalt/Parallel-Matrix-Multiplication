#include <stdio.h>
#include <stdlib.h>
#include "error_handling.h"

void checkArgsSize(int argSize) {
	if (argSize != 2) {
		printf("É necessário passar o parâmetro da quantidade de processos.\n");
		exit(EXIT_FAILURE);
	}
}

void checkInputMatrices(Matrix* m1, Matrix* m2) {
	if (m1->columns != m2->rows) {
		printf("A matriz 1 tem quantidade de colunas diferente da quantidade de linhas da matriz 2, portanto não é possível efetuar a multiplicação delas.\n");
		exit(EXIT_FAILURE);
	}
}

void checkNumTasks(char** argv, int* numParallelTasks, Matrix* m1) {
	sscanf(argv[1], "%d", numParallelTasks);
	if (m1->rows < (*numParallelTasks)) {
		printf("O número de processos/threads não pode ser maior que a quantidade de linhas da primeira matriz de entrada.\n");
		exit(EXIT_FAILURE);
	}

	if ((*numParallelTasks) < 1) {
		printf("O número de processos/threads tem que ser maior que 0.\n");
		exit(EXIT_FAILURE);
	}
}