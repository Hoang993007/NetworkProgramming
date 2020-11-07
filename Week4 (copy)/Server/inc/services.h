#ifndef __SERVICES_H__
#define __SERVICES_H__

#include <stdio.h>
#include "accountSystem.h"

#define WRONG_ACTIVECODE_BLOCK_COUNT_DOWN 3
#define WRONG_PASS_BLOCK_COUNT_DOWN 3

int init(char* inputFile);
int services(int choice);
void accountRegister ();
account* getAccount ();
void activate ();
void logIn ();
int isLogined ();
void search ();
void changePass ();
void signOut ();

#endif
