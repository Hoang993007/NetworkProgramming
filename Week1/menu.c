#include <stdio.h>
#include "menu.h"

int menu(char* inputFile) {
  if(openInputStream(inputFile) == IO_ERROR) {
    printf("Cann\'t read input file!\n");
    return -1;
  };
  
  int choice;
  do {
    menu();

    scanf("%d", &choice); 

    switch(choice){
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
    default:
      break;
    }
  }while(choice != 6);
}

void printMenu () {
  printf("USER MANAGEMENT PROGRAM");
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
  printf("Your choice (1-6, orther to quit):");
}

void accountRegister () {
  // 1. read data from keyboard

  // 2. check ifExist
  /*
    if exist => error
  */

  // 3. add to user list. status = idle

}

void activate () {
  // 1. read data from keyboard

  // 2. wrong activation code => error. Exeeds 5 times => block

  // 3. active account

}

void sigIn  () {
  // 1. read data from keyboard

  // 2. check if exist and valid pass


}

void search () {
}

void changePass () {
}

void signOut () {
}
