#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../inc/menu.h"
#include "../inc/accountSystem.h"

userNameType* logedIn = NULL;
int logedInCount = 0;

int menu(char* inputFile) {
  if(openInputStream(inputFile) == IO_ERROR) {
    printf("Cann\'t read input file!\n");
    return -1;
  };

  int choice;
  do {
    printMenu();

    scanf("%d", &choice); 

    printf("-------------------------------");
    printf("\n");

    switch(choice){
    case 1:
      accountRegister();
      break;
    case 2:
      activate();
      break;
    case 3:
      logIn ();
      break;
    case 4:
      search();
      break;
    case 5:
      changePass();
      break;
    case 6:
      signOut();
      break;
    default:
      printf ("Quit");
      printf("\n");
      break;
    }
    storeAccountDataToFile(inputFile);
    printf("-------------------------------");
    printf("\n");
  }while(choice >= 1 && choice <= 6);

  rubbishCollection();

  //another rubbish collection
}

void printMenu () {
  printf("USER MANAGEMENT PROGRAM");
  printf("\n");
  printf("-------------------------------");
  printf("\n");
  printf("1. Register");
  printf("\n");
  printf("2. Activate");
  printf("\n");
  printf("3. Sign in");
  printf("\n");
  printf("4. Search");
  printf("\n");
  printf("5. Change password");
  printf("\n");
  printf("6. Sign out");
  printf("\n");
  printf("Your choice (1-6, orther to quit): ");
}

void accountRegister () {
  // 1. read data from keyboard
  char newUserName[MAX_NAME_LENGTH];
  char password[MAX_PASS_LENGTH];

  printf ("Username: ");
  scanf ("%s", newUserName);

  // 2. check ifExist
  if(isExist(newUserName) == -1) {
    printf ("Password: ");
    scanf ("%s", password);

    // 3. add to user list. status = idle
    addNode(newUserName, password, 2);
    printf("Successful registration. Activation required.");
    printf("\n");
    printDB();

  } else {
    printf ("Account existed\n");
    return;
  }
}

void activate () {
  account* signInAccount = accessAcc ();
  if(signInAccount != NULL) {
    activateAccount(signInAccount);
  }
}

void logIn  () {
  account* logInAccount = accessAcc ();

  if(logInAccount != NULL) {
    if(isLogedIn(logInAccount->userName) == 0) {
      printf ("This account already loged in");
      printf ("\n");
      return;
    }

    if(logedInCount == 0) {
      logedIn = (userNameType*)malloc(sizeof(userNameType));
      logedInCount++;
    } else {
      logedIn = realloc(logedIn, sizeof(userNameType)*(logedInCount+1));
      logedInCount++;
    }

    strcpy(logedIn[logedInCount-1], logInAccount->userName);
    printLogedInAccount ();
    printf ("Hello %s", logInAccount->userName);
    printf("\n");
  }
}

int isLogedIn (char* userName) {
  for(int i=0; i < logedInCount; i++) {
    if(strcmp(logedIn[i], userName) == 0)
      return 0;
  }
  return -1;
}

void printLogedInAccount() {
  printf ("---------------------------------");
  printf("\n");
  printf ("ACCOUNT LOGED IN:");
  printf("\n");
  printf("\n");
  for(int i=0; i < logedInCount; i++) {
    printf ("%s\n", logedIn[i]);
  }
  printf ("---------------------------------");
  printf("\n");
}

account* accessAcc () {
  // 1. read data from keyboard
  char userName[MAX_NAME_LENGTH];
  char password[MAX_PASS_LENGTH];

  printf ("Username: ");
  scanf ("%s", userName);

  if(isExist(userName) == -1) {
    printf ("Cannot find account\n");
    return NULL;
  }

  account* accountAccess = searchAccount(userName);

  if(accountAccess->status == 0) {
    printf ("Account is blocked");
    printf("\n");
    return NULL;
  }

  // 2. check ifExist

  printf ("Password: ");
  scanf ("%s", password);

  int checkPassRes = checkPassword(accountAccess, password);

  if(checkPassRes == 0)
    return accountAccess;
  else {
    if(checkPassRes == -1)
      printf ("Password is incorrect\n");
    else if(checkPassRes == -2)
      printf ("Account is blocked\n");

    return NULL;
  }
}

void search () {
  if(logedInCount == 0) {
    printf ("You haven't loged in");
    printf("\n");
    return;
  }

  // 1. read data from keyboard
  char userName[MAX_NAME_LENGTH];
  char password[MAX_PASS_LENGTH];

  printf ("Username: ");
  scanf ("%s", userName);

  if(isExist(userName) == -1) {
    printf ("Cannot find account\n");
    return;
  }

  account* accountAccess = searchAccount(userName);

  if(accountAccess->status == 0) {
    printf ("Account is blocked");
    printf("\n");
  } else if(accountAccess->status == 2) {
    printf ("Account is not activated");
    printf("\n");
  } else if(accountAccess->status == 1) {
    printf ("Account is active");
    printf("\n");
  }
}

void changePass () {
  // 1. read data from keyboard
  char userName[MAX_NAME_LENGTH];
  char password[MAX_PASS_LENGTH];
  char newPassword[MAX_PASS_LENGTH];

  printf ("Username: ");
  scanf ("%s", userName);

  if(isExist(userName) == -1) {
    printf ("Account is not sign in\n");
    return;
  }

  if(isLogedIn(userName) == -1) {
    printf ("Account is not sign in");
    printf("\n");
    return;
  }

  account* accountAccess = searchAccount(userName);

  if(accountAccess->status == 0) {
    printf ("Account is blocked");
    printf("\n");
    return;
  }

  // 2. check ifExist

  printf ("Password: ");
  scanf ("%s", password);

  printf ("NewPassword: ");
  scanf ("%s", newPassword);

  int checkPassRes = checkPassword(accountAccess, password);

  if(checkPassRes == 0) {
    accountChangePass(accountAccess, newPassword);
    printDB();
    printf ("Password is changed");
    printf("\n");
  } else {
    if(checkPassRes == -1)
      printf ("Current password is incorrect, please try again\n");
    else if(checkPassRes == -2)
      printf ("Account is blocked\n");
  }
}

void signOut () {
   // 1. read data from keyboard
  char userName[MAX_NAME_LENGTH];

  printf ("Username: ");
  scanf ("%s", userName);

  if(isExist(userName) == -1) {
    printf ("Cannot find account\n");
    return;
  }

  if(isLogedIn(userName) == -1) {
    printf ("Account is not sign in");
    printf("\n");
    return;
  }

  printf ("Goodbye %s", userName);
  printf("\n");

  for(int i=0; i < logedInCount; i++) {
    if(strcmp(logedIn[i], userName) == 0) {
      for(int j = i+1; j < logedInCount; j++)
	strcpy(logedIn[j-1],logedIn[j]);
      logedInCount--;
      logedIn = realloc(logedIn, sizeof(userNameType)*logedInCount);
      printLogedInAccount();
      break;
    }
  }
}
