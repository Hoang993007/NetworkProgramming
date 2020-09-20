#ifndef __ACCOUNTSYSTEM_H__
#define __ACCOUNTSYSTEM_H__

#include <stdio.h>
#define IO_ERROR 0
#define IO_SUCCESS 1

#define MAX_NAME_LENGTH 50
#define MAX_PASS_LENGTH 20
#define ACTIVE_CODE_LENGTH 20

extern char ACTIVE_CODE[ACTIVE_CODE_LENGTH];

typedef char userNameType[MAX_NAME_LENGTH] ;
typedef char passwordType[MAX_NAME_LENGTH] ;

/*status: 1: active  - 0: blocked -  2: idle*/
typedef struct _account {
  userNameType userName;
  passwordType password;
  int status;
  int wrongActiveCode;
  int wrongPass;
}account;

typedef struct _node {
  account data;
  struct _node* next;
}node;

int openInputStream(char* fileName);
void rubbishCollection();
void closeInputStream();
node* createAccount_node (char* userName, char* password, int status);
void addNode (char* userName, char* password, int status);
node* findNode (char* userName);
void deleteNode (char* userName);
void printDB ();
void loadAccountData ();
int storeAccountDataToFile (char* fileName);
int isExist(char* userName);
int checkPassword(account* accountCheck, char* password);
void activateAccount (account* accountActivate);
account* searchAccount (char* userName);
void accountChangePass (account* accountModify, char* newPass);

#endif
