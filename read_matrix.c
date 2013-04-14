#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_MAX_SIZE 10

char* trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}

int* readVectorFromString(char* line, int columns) {
	int i = 0, charIndex = 0, lineSize = strlen(line), column = 0;
	char* temp = (char*) calloc(NUMBER_MAX_SIZE, sizeof(char));
	int currentNumber = 0;
	int* lineNumbers = (int*) calloc(columns, sizeof(int));
	
	for (i = 0; i < lineSize; i++) {
		if (line[i] == 32 || i == (lineSize-1)) {
			temp = trimwhitespace(temp);
    	printf("Here is the string, %s\n", temp);
    	sscanf(temp, "%d", &currentNumber);
    	temp = (char*) calloc(NUMBER_MAX_SIZE, sizeof(char));
    	lineNumbers[column] = currentNumber;
      charIndex = 0;
      column++;
		} else {
			temp[charIndex] = line[i]; 
      charIndex++;
		}
	}
	
	free(temp);
	
	return lineNumbers;
}

int** readMatrixFromFile(char* filePath) {
	FILE *m;
	char* line = NULL;
  size_t len = 0;
  ssize_t read;
  int lineIndex = 1, matrixLineIndex = 0, matrixLines = 0, matrixColumns = 0, successParsing = 1;
  int** matrix;
    
  m = fopen(filePath, "r");
  
  if (m == NULL)
		exit(EXIT_FAILURE);
  
  while ((read = getline(&line, &len, m)) != -1) {		
		if (lineIndex == 1) {
			successParsing = sscanf(line, "LINHAS = %d", &matrixLines);
			if (!successParsing)
				exit(EXIT_FAILURE);
			matrix = (int**) calloc(matrixLines, sizeof(int*));
		} else if (lineIndex == 2) {
			successParsing = sscanf(line, "COLUNAS = %d", &matrixColumns);
			if (!successParsing)
				exit(EXIT_FAILURE);
		} else {
			matrix[matrixLineIndex] = readVectorFromString(line, matrixColumns);
		}
					
		lineIndex++;
	}
	
	printf("linhas: %d\n", matrixLines);
	printf("colunas: %d\n", matrixColumns);
	
	if (line)
		free(line);
		
	fclose(m);
}
