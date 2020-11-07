#ifndef __ACCOUNTSYSTEM_H__
#define __ACCOUNTSYSTEM_H__

#include <stdio.h>
#include "errorCode.h"

#define PRINT_DB 1

extern char ACTIVE_CODE[ACTIVE_CODE_LENGTH];

extern char inputFilePath[255];
extern FILE* inputFile;

extern char outputFilePath[255];
extern FILE* outputFile;

typedef enum
{    IDLE,
  ACTIVE,
    BLOCKED
}accountStatus;

typedef char userNameType[MAX_USERNAME_LENGTH] ;
typedef char passwordType[MAX_PASS_LENGTH] ;

/*status: 1: active  - 2: blocked -  0: idle*/
typedef struct _account {
  userNameType userName;
  passwordType password;
  accountStatus status;
  int wrongActiveCode;
  int wrongPass;
}account;

typedef struct _node {
  account data;
  struct _node* next;
}node;

extern node* front;
extern node* rear;

int openInputStream(char* filePath);
void rubbishCollection();
void closeInputStream();
// add new account -----------------------------------------
account newAccount (char* userName, char* password, accountStatus status);
void addNewAccountToDB (account newAccount);
node* newAccount_node (account newAccount);
void addNode (node* newNode);
// ============================================
account* getAccountByUserName (char* userName);
node* getNodeByUserName (char* userName);
int isExistUserName(char* userName);
account* getAccount (userNameType userName, passwordType password, int *res);
// ===================================
void accountChangePass (account* accountModify, char* newPass);
// ============================================
void deleteAccountByUserName (char* userName);
void deleteNodeByUserName (char* userName);
// ===========================================================
void loadDataFromFile ();
int storeAccountDataToFile (char* filePath);
// ==============================================================
int checkPassword(account* accountCheck, char* password);
// ==============================================================
/*status: 1: active  - 0: blocked -  2: idle*/
int activateAccount (account* accountActivate);
void printDB();
// =============================================================

#endif
