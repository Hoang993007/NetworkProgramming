#ifndef __SERVICES_H__
#define __SERVICES_H__

#include <stdio.h>
#include "accountSystem.h"
#include "errorCode.h"

#define WRONG_ACTIVECODE_BLOCK_COUNT_DOWN 3
#define WRONG_PASS_BLOCK_COUNT_DOWN 3

#define PRINT_LOGEDIN 1

#define LOGED_IN 60
#define NOT_LOGED_IN 61

typedef struct _accessPermit {
 char IP[255];
 userNameType userName;
}accessPermit;

extern int logedInCount;

int init(char* inputFile);
void accountRegister ();
void activate (userNameType userName, passwordType password);
int logIn (char* IP, userNameType userName, passwordType password);
int isLogedIn (char* IP);
accessPermit* logInInfoGet(char* IP);
void search ();
void changePass (char* IP, passwordType newPassword);
void signOut (char* IP);
void printLogedInAccount();
void freeLogIn();

#endif
