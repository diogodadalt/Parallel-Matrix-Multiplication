#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "write_matrix.h"

void writeMatrixInFile(char* filePath, Matrix* matrix) {
	FILE* output;
	int rows = matrix->rows;
	int columns = matrix->columns;
	int** matrixData = matrix->data;
	int i = 0, j = 0;
	
	output = fopen(filePath, "w");
	if (output == NULL) {
     printf("Não foi possível abrir o arquivo de saída para escrita.\n");
     exit(EXIT_FAILURE);
  }

	fprintf(output, "LINHAS = %d\n", rows);
	fprintf(output, "COLUNAS = %d\n", columns);
  
  for (i = 0; i < rows; i++) {
  	for (j = 0; j < columns; j++) {
  		fprintf(output, "%d", matrixData[i][j]);
  		if (j < (columns - 1))
  			fprintf(output, " ");  			
  	}
  	if (i < (rows - 1))
	  	fprintf(output, "\n");
  }
    
  fclose(output);
}

void writeMatrixInOutput(Matrix* matrix) {
	int rows = matrix->rows;
	int columns = matrix->columns;
	int** matrixData = matrix->data;
	int i = 0, j = 0;

	printf("\n");
	printf("process PID (%d)\n", getpid());
	printf("LINHAS = %d\n", rows);
	printf("COLUNAS = %d\n", columns);
  
  for (i = 0; i < rows; i++) {
  	for (j = 0; j < columns; j++) {
  		printf("%d", matrixData[i][j]);
  		if (j < (columns - 1))
  			printf(" ");  			
  	}
  	if (i < (rows - 1))
	  	printf("\n");
  }
  
  printf("\n");
}
