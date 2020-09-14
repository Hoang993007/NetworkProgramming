#include <stdio.h>
#include "accountSystem.h"

FILE* inputFile;
node* first;

int openInputStream(char* fileName) {
  inputFile = fopen(fileName, "r+");
  if(inputFile == NULL)
    return IO_ERROR;
  return IO_SUCCESS;
}

void rubbishCollection() {
  node* tmp = first;
  
  while(*tmp != NULL) {
    node* trash = tmp;
    tmp = trash->next;
    free(tmp);
  }
}

void closeInputStream() {
  fclose(inputFile);
}
