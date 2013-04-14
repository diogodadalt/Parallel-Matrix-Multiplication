#include <stdio.h>
#include <stdlib.h>
#include "read_matrix.h"

#define M1 "entradas/m1.txt"
#define M2 "entradas/m1.txt"
//#define M1 "in1.txt"
//#define M2 "in2.txt"

int main(int argc, char** argv) {

	int **m1 = NULL, **m2 = NULL;

	if (argc != 2) {
		printf("É necessário passar o parâmetro da quantidade de processos.\n");
		exit(EXIT_FAILURE);
	}
	
	m1 = readMatrixFromFile(M1);
	m2 = readMatrixFromFile(M2);
	 
	return 0;
}
