#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/accountSystem.h"

FILE* inputFile;
FILE* outputFile;
node* front = NULL;
node* rear = NULL;

char ACTIVE_CODE[ACTIVE_CODE_LENGTH] = "LTM121216";

int openInputStream(char* fileName) {
  inputFile = fopen(fileName, "r+");
  if(inputFile == NULL) {
    return IO_ERROR;
  } else {
    loadAccountData();
    return IO_SUCCESS;
  }
}

void rubbishCollection() {
  node* tmp = front;
  
  while(tmp != NULL) {
    node* trash = tmp;
    tmp = trash->next;
    free(tmp);
  }
}

void closeInputStream() {
  fclose(inputFile);
}

node* createAccount_node (char* userName, char* password, int status) {
  userName[strlen(userName)] = '\0';
  password[strlen(password)] = '\0';

  node* tmp = (node*)malloc(sizeof(node));
  strcpy(tmp->data.userName, userName);
  strcpy(tmp->data.password, password);
  tmp->data.status = status;
  tmp->data.wrongActiveCode = 0;
  tmp->data.wrongPass = 0;
  tmp->next = NULL;
};

void addNode (char* userName, char* password, int status) {
  node* addNode = createAccount_node (userName, password, status);

  if(front == NULL) {
    front = addNode;
    rear = front;
  } else if (front != NULL) {
    rear->next = addNode;
    rear = addNode;
  }
}

node* findNode (char* userName) {
  node* tmp = front;

  while(tmp != NULL) {
    node* tmp2 = tmp;
    tmp = tmp2->next;
    if (strcmp(tmp2->data.userName, userName) == 0)
      return tmp2;
  }

  return NULL;
}

void deleteNode (char* userName) {
  node* tmp = front;

  if(strcmp(tmp->data.userName, userName) == 0) {
    front = tmp->next;
    free(tmp);
  } else {
    node* tmp2 = tmp;
    tmp = tmp->next;

    do{
      if((tmp->data.userName, userName) == 0) {
	tmp2->next = tmp->next;
	free(tmp);
	break;
      }

      tmp2 = tmp;
      tmp = tmp->next;
    }while(tmp != NULL);
  }
}

void printDB () {
  printf("-----------------------------");
  printf("\n");
  printf("Database");
  printf("\n\n");
  node* tmp = front;

  while(tmp != NULL) {
    node* tmp2 = tmp;
    tmp = tmp2->next;
    printf("%s %s %d\n", tmp2->data.userName, tmp2->data.password, tmp2->data.status);
  }

  printf("-----------------------------");
  printf("\n");
}

void loadAccountData () {
  char buf[1024];
  
  while((fgets(buf, sizeof(buf), inputFile)) != NULL) {

    // check if data was just read is valid or not

    //strtok function
    
    /*Returns */

    /* The strtok function returns a pointer to the first character of the token. If no token is found, a null pointer is returned. */

    /* You can find additional tokens in the same string using strtok(NULL, s2) because the strtok function continues the search begun by the previous strtok call. */

    char * userName;
    char * password;
    int status;

    userName = strtok(buf, " ");
    password = strtok(NULL, " ");
    char * tmp = strtok(NULL, " ");
    status = atoi(tmp);

    addNode(userName, password, status);
  }

  printDB();

  closeInputStream();
};

int storeAccountDataToFile (char* fileName) {
  outputFile = fopen(fileName, "w");
  if(outputFile == NULL) {
    printf("Cann\'t write into output file!\n");
    return IO_ERROR;
  } else {
    node* tmp = front;

    while(tmp != NULL) {
      node* tmp2 = tmp;
      tmp = tmp2->next;

      fprintf(outputFile,"%s %s %d\n", tmp2->data.userName, tmp2->data.password, tmp2->data.status);
    }
  }
  fclose(outputFile);
  return IO_SUCCESS;
}

int isExist(char* userName) {
  if(findNode (userName) != NULL)
    return 0;
  return -1;
};

int checkPassword(account* accountCheck, char* password) {
  if(accountCheck->status == 0)
    return -2;// account blocked
  else if(strcmp(password, accountCheck->password) == 0)
    return 0;// correct password
  else {
    accountCheck->wrongPass ++;
    if(accountCheck->wrongPass == 3) {
      accountCheck->status = 0;
      return -2;
    }
    else return -1;
  }
};

void activateAccount (account* accountActivate) {
  switch (accountActivate->status) {
  case 0:
    printf ("Account is blocked.");
    printf("\n");
    break;
  case 1:
    printf ("This account is already activated!");
    printf("\n");
    break;
  case 2:
    printf ("Code: ");
    char activeCode[ACTIVE_CODE_LENGTH];
    scanf ("%s", activeCode);

    if(strcmp(activeCode, ACTIVE_CODE) == 0) {
      accountActivate->status = 1;
      printf ("Account is activated");
      printf("\n");
    } else {
      accountActivate->wrongActiveCode ++;
      if(accountActivate->wrongActiveCode < 3) {
	printf ("Account is not activated");
	printf("\n");
      } else if (accountActivate->wrongActiveCode == 3) {
	accountActivate->status = 0;
	printf ("Activation code is incorrect!");
	printf("\n");
	printf("Account is blocked");
	printf("\n");
      }
    }
    break;
  }
}

account* searchAccount (char* userName) {
  node* findNodeRes = findNode(userName);
  if(findNodeRes == NULL)
    return NULL;
  else
    return &(findNodeRes->data);
};

void accountChangePass (account* accountModify, char* newPass) {
  strcpy(accountModify->password, newPass);
};
