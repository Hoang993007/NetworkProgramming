#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <errno.h>
#include "./inc/services.h"
#include "./inc/accountSystem.h"

#define BUFF_SIZE 255

int receiveFromClient(int sockfd, struct sockaddr_in* cliaddr, char* buff) {
   int rcvBytes;
  socklen_t len;
    len = sizeof(*cliaddr);

    rcvBytes = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *) cliaddr, &len);

    if(rcvBytes < 0){
      perror("Error: ");
      return 0;
    }

    buff[rcvBytes] = '\0';

    printf("[%s:%d]:\n\t%s\n", inet_ntoa(cliaddr->sin_addr), ntohs(cliaddr->sin_port), buff);
}

int sendToClient(int sockfd, struct sockaddr_in* cliaddr, char* buff) {
   int sendBytes;
  socklen_t len;
    len = sizeof(*cliaddr);

    sendBytes = sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr *) cliaddr, len);

    if(sendBytes < 0){
      perror("Error: "); return 0;
    }

}

int main(int argc, char *argv[]) {
  init("./Server/bin/nguoidung.txt");

  int SERV_PORT = atoi(argv[1]);

  int sockfd;
    socklen_t len;

    char buff[BUFF_SIZE];

  struct sockaddr_in servaddr, cliaddr;

  bzero(&servaddr, sizeof(servaddr));

  printf("Construct socket...\n");

  //Step 1: Construct socket
  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("Error: "); 
    return 0;
  }

  printf("Bind address to socket...\n");

  //Step 2: Bind address to socket
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))){
    perror("Error: ");
    return 0;
  }

  printf("Server started.\n");

printf("\n");
  printf("Communicate with client...\n");

  //Step 3: Communicate with client
  /* now loop, receiving data and printing what we received */

  int service;
  char userName[255];
  char password[255];
char xxx[255];
for(;;){
receiveFromClient(sockfd, &cliaddr, buff);
service = atoi(buff);

    switch(service){
    case 1:
      //accountRegister();
      break;
    case 2:
      //activate();
      break;
    case 3:
   receiveFromClient(sockfd, &cliaddr, userName);
   receiveFromClient(sockfd, &cliaddr, password);
     if(isLogedIn(inet_ntoa(cliaddr.sin_addr)) == NOT_LOGED_IN) {
        int res = logIn (inet_ntoa(cliaddr.sin_addr), userName, password);
printf("%d\n", res);
         if(res == LOGIN_SUCCESS){
sendToClient(sockfd, &cliaddr, "OK");
sendToClient(sockfd, &cliaddr, "\n");
}
else if(res == ACCOUNT_BLOCKED) {
sendToClient(sockfd, &cliaddr, "Account ");
sendToClient(sockfd, &cliaddr, "is ");
sendToClient(sockfd, &cliaddr, "blocked");
sendToClient(sockfd, &cliaddr, "\n");
}
else if(res == ACCOUNT_IDLE) {
sendToClient(sockfd, &cliaddr, "Account ");
sendToClient(sockfd, &cliaddr, "not ");
sendToClient(sockfd, &cliaddr, "ready");
sendToClient(sockfd, &cliaddr, "\n");
}
else {
sendToClient(sockfd, &cliaddr, "NOT OK");
sendToClient(sockfd, &cliaddr, "\n");
}
}
      else printf("You have already loged in\n");
      break;/*
    case 4:
      if(isLogedIn(IP) == LOGED_IN)
        search();
      else printf("You have not loged in\n");
      break;
    case 5:
      if(isLogedIn(IP) == LOGED_IN)
        changePass(IP);
      else printf("You have not loged in\n");
      break;
    case 6:
      if(isLogedIn(IP) == LOGED_IN)
        signOut(IP);
      else printf("You have not loged in\n");
      break;*/

case 7:
 receiveFromClient(sockfd, &cliaddr, xxx);
sendToClient(sockfd, &cliaddr, xxx);
sendToClient(sockfd, &cliaddr, "\n");
break;
    }
}

  printf("Closing socket...\n");

  close (sockfd);
  // không đóng => mất socket

  rubbishCollection();
  //another rubbish collection
  return 0;
}
