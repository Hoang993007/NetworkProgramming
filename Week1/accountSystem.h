#ifndef __ACCOUNTSYSTEM_H__
#define __ACCOUNTSYSTEM_H__

#include <stdio.h>
#define IO_ERROR 0
#define IO_SUCCESS 1

/*status: 1: active  - 0: blocked -  2: idle*/
typedef struct _account {
  string userName;
  string password;
  int status;
}account;

typedef struct _node {
  account data;
  _node* next;
}node;

int openInputStream(char* fileName);

void closeInputStream();

void rubbishCollection();

void loadAccountData ();

void storeAccountData ();

int isExist(string userName);

int checkPassword(string userName, string password);

account* createNewAccount (string userName, string password, int status);

account* searchAccount (string userName);

void changePass (string userName);

#endif
