#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "matrix.h"
#include "read_matrix.h"
#include "write_matrix.h"

int main(int argc, char** argv) {

	Matrix *m1 = NULL, *m2 = NULL;

	if (argc != 2) {
		printf("É necessário passar o parâmetro da quantidade de processos.\n");
		exit(EXIT_FAILURE);
	}
	
	m1 = readMatrixFromFile(M1);
	m2 = readMatrixFromFile(M2);
	 
	if (m1->rows != m2->columns) {
		printf("A matriz 1 tem quantidade de linhas diferente da quantidade de colunas da matriz 2, portanto não é possível efetuar a multiplicação delas.\n");
		exit(EXIT_FAILURE);
	}		
	 
	return 0;
}
