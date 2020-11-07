#ifndef __MENU_H__
#define __MENU_H__

#include <stdio.h>
#include "accountSystem.h"
#define WRONG_ACTIVECODE_BLOCK_COUNT_DOWN 3
#define WRONG_PASS_BLOCK_COUNT_DOWN 3

int menu(char* inputFile);
void printMenu ();
void accountRegister ();
void activate ();
void logIn ();
int isLogedIn (char* userName);
void printLogedInAccount();
account* accessAcc ();
void search ();
void changePass (); 
void signOut ();

#endif
