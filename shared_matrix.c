#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include "shared_matrix.h"

Matrix* createSharedMatrix(int rows, int columns) {
	int shmid, i;
	Matrix* result = (Matrix*) malloc(sizeof(Matrix));
	int** matrix = (int**) calloc(rows, sizeof(int*));
	
	for (i = 0; i < rows; i++) {
		if ((shmid = shmget(SHM_KEY + i, columns*sizeof(int), IPC_CREAT | 0666)) < 0) {
			printf("Erro na criação da memória compartilhada.\n");
			perror("shmget");
			exit(1);
		}		 
		if ((matrix[i] = (int*)shmat(shmid, NULL, 0)) == (int*) -1) {
			printf("Erro ao tentar se atachar a memória compartilhada.\n");
		  perror("shmat");
		  exit(1);
		}
	}
	  
	result->data = matrix;
	result->rows = rows;
	result->columns = columns;  
  return result;
}

Matrix* readSharedMatrix(int rows, int columns) {
	int shmid, i;
	Matrix* result = (Matrix*) malloc(sizeof(Matrix));
	int** matrix = (int**) calloc(rows, sizeof(int*));
	
	for (i = 0; i < rows; i++) {
		if ((shmid = shmget(SHM_KEY + i, columns*sizeof(int), 0666)) < 0) {
			printf("Erro ao tentar localizar a memória compartilhada.\n");
		  perror("shmget");
		  exit(1);
		}		
		if ((matrix[i] = (int*)shmat(shmid, NULL, 0)) == (int*) -1) {
			printf("Erro ao tentar se atachar a memória compartilhada.\n");
		  perror("shmat");
		  exit(1);
		}
  }
  
	result->data = matrix;
	result->rows = rows;
	result->columns = columns;  
  return result;
}

void detach(Matrix* matrix) {
	int i;
	for (i = 0; i < matrix->rows; i++) {
		if (shmdt(matrix->data[0]) == -1) {
			printf("Erro ao tentar se desatachar da memória compartilhada.\n");
		  perror("shmdt");
		}
	}
}
