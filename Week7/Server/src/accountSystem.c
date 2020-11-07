#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/accountSystem.h"

char ACTIVE_CODE[ACTIVE_CODE_LENGTH] = "LTM121216";

char inputFilePath[255];
FILE* inputFile = NULL;

char outputFilePath[255];
FILE* outputFile = NULL;

node* front = NULL;
node* rear = NULL;

int openInputStream(char* filePath) {
  strcpy(inputFilePath, filePath);
  strcpy(outputFilePath, inputFilePath); // FIle input and File input are the same

  inputFile = fopen(filePath, "r+");

  if(inputFile == NULL) {
    return IO_ERROR;

  } else {
    loadDataFromFile ();
    //fclose(inputFile);
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

// add new account -----------------------------------------
account newAccount (char* userName, char* password, accountStatus status) {
  account newAccount;

  userName[strlen(userName)] = '\0';
  password[strlen(password)] = '\0';

  strcpy(newAccount.userName, userName);
  strcpy(newAccount.password, password);
  newAccount.status = status;
  newAccount.wrongActiveCode = 0;
  newAccount.wrongPass = 0;

  addNewAccountToDB(newAccount);
}

void addNewAccountToDB (account newAccount) {
  node* newNode = newAccount_node (newAccount);
  addNode(newNode);
}

node* newAccount_node (account newAccount) {
  node* tmp = (node*)malloc(sizeof(node));
  tmp->data = newAccount;
  tmp->next = NULL;
}

void addNode (node* newNode) {
  if(front == NULL) {
    front = newNode;
    rear = front;
  } else if (front != NULL) {
    rear->next = newNode;
    rear = newNode;
  }
}

// ============================================
account* getAccountByUserName (char* userName) {
   node* accountNode = getNodeByUserName(userName);
   if(accountNode == NULL) return NULL;
   else return &(accountNode->data);
}

node* getNodeByUserName (char* userName) {
  node* tmp = front;

  while(tmp != NULL) {
    node* tmp2 = tmp;
    tmp = tmp2->next;
    if (strcmp(tmp2->data.userName, userName) == 0)
      return tmp2;
  }

  return NULL;
}

int isExistUserName(char* userName) {
  if(getAccountByUserName(userName) != NULL)
    return ACCOUNT_EXIST;
  return ACCOUNT_NOT_EXIST;
}

// ===================================

account* getAccount (userNameType userName, passwordType password, int *res) {
  if(isExistUserName(userName) == ACCOUNT_NOT_EXIST) {
    printf ("%s\n", "Error: Account not exits");
    return NULL;
  }

  account* accountAccess = getAccountByUserName(userName);

  if(accountAccess->status == BLOCKED) {
    printf ("Account is blocked");
    printf("\n");
    return NULL;
  }

  *res = checkPassword(accountAccess, password);

  if(*res == PASS_CORRECT){
    printf("get account %s sucess\n", accountAccess->userName);
    return accountAccess;
  }
  else {
    if(*res == PASS_INCORRECT) {
      printf ("Password is incorrect\n");
    }
    else if(*res == ACCOUNT_BLOCKED) {
      printf ("Account is blocked\n");
    }
    else if(*res == ACCOUNT_IDLE) {
      printf ("Account is not activated\n");
    }

    return NULL;
  }
}

// ===================================
void accountChangePass (account* accountModify, char* newPass) {
  strcpy(accountModify->password, newPass);
printDB();
  storeAccountDataToFile(outputFilePath);
};

// ============================================
void deleteAccountByUserName (char* userName) {
  deleteNodeByUserName(userName);
    storeAccountDataToFile(outputFilePath);
}

void deleteNodeByUserName (char* userName) {
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

// ===========================================================
void loadDataFromFile () {
  char buf[1024];

  int line = 0;

  while((fgets(buf, sizeof(buf), inputFile)) != NULL) {
    line++;

    int checkSpace = 0;
    for(int i=0; i < strlen(buf); i++) {
      if(buf[i] == ' ') checkSpace ++;
    }

    if(checkSpace != 2) {
      printf ("Line %d: Invalid account information!", line);
      printf("\n");
      continue;
    }

    char * userName;
    char * password;
    accountStatus status;

    userName = strtok(buf, " ");
    password = strtok(NULL, " ");
    char * tmp = strtok(NULL, " ");
    tmp[strlen(tmp)-1] = '\0';

    if(strlen(tmp) > 1 || (tmp[0] > '2' || tmp[0] < '0')) {
      printf ("Line %d: There's an error in account status!", line);
      printf("\n");
      continue;
    }

    status = atoi(tmp);

    newAccount (userName, password, status);
  }

  if(PRINT_DB == 1) printDB();
};

int storeAccountDataToFile (char* filePath) {
  outputFile = fopen(filePath, "w");

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

// ==============================================================
int checkPassword(account* accountCheck, char* password) {
  if(accountCheck->status == IDLE){
printf("idle account\n");
    return ACCOUNT_IDLE;}
else if(accountCheck->status == BLOCKED)
    return ACCOUNT_BLOCKED;
  else if(strcmp(password, accountCheck->password) == 0)
    return PASS_CORRECT;
  else {
    accountCheck->wrongPass ++;

    if(accountCheck->wrongPass == 3) {
      accountCheck->status = BLOCKED;
    storeAccountDataToFile(outputFilePath);
      return ACCOUNT_JUST_BLOCKED;
    }

    return PASS_INCORRECT;
  }
};

// ==============================================================
/*status: 1: active  - 0: blocked -  2: idle*/
int activateAccount (account* accountActivate) {
  switch (accountActivate->status) {
  case 0:
    return ACCOUNT_BLOCKED;
    break;
  case 1:
    return ACCOUNT_ACTIVE;
    break;
  case 2:
    printf ("Code: ");
    char activeCode[ACTIVE_CODE_LENGTH];
    scanf ("%s", activeCode);
    getchar();

    if(strcmp(activeCode, ACTIVE_CODE) == 0) {
      accountActivate->status = 1;
    storeAccountDataToFile(outputFilePath);
      printf ("Account is activated");
      printf("\n");
      return ACTIVE_SUCCESS;
    } else {
      accountActivate->wrongActiveCode ++;

      if(accountActivate->wrongActiveCode < 3) {
	printf ("Account is not activated");
	printf("\n");
        return ACTIVE_ERROR;

      } else if (accountActivate->wrongActiveCode == 3) {
	accountActivate->status = 0;
	printf ("Activation code is incorrect!");
	printf("\n");
	printf("Account is blocked");
	printf("\n");
        return ACCOUNT_BLOCKED;
      }
    }
    break;
  }
}

void printDB() {
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

// =============================================================
