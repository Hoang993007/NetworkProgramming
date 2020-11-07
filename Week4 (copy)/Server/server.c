#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <errno.h>
#include "inc/services.h"

int main(int argc, char *argv[]) {
  init("./bin/nguoidung.txt");

  int SERV_PORT = atoi(argv[1]);

  int sockfd, rcvBytes, sendBytes;
  socklen_t len;

  struct sockaddr_in servaddr, cliaddr;

  bzero(&servaddr, sizeof(servaddr));

  //Step 1: Construct socket
  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("Error: "); 
    return 0;
  }

  //Step 2: Bind address to socket
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))){
    perror("Error: ");
    return 0;
  }

  printf("Server started.\n");

  //Step 3: Communicate with client
  /* now loop, receiving data and printing what we received */

  char userName[MAX_USERNAME_LENGTH];
  char password[MAX_PASS_LENGTH];

  for ( ; ; ) {/*
    len = sizeof(cliaddr);

    rcvBytes = recvfrom(sockfd, buff, BUFF_SIZE, 0, (struct sockaddr *) &cliaddr, &len);

    if(rcvBytes < 0){
      perror("Error: ");
      return 0;
    }

    char mesg[] = "Success";
    buff[rcvBytes] = '\0';

    printf("[%s:%d]: %s\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), mesg);

    sendBytes = sendto(sockfd, buff, rcvBytes, 0, (struct sockaddr *) &cliaddr, len);
    if(sendBytes < 0){
      perror("Error: "); return 0;
    }*/
  }

  close (sockfd);
  // không đóng => mất socket

  rubbishCollection();
  //another rubbish collection
  return 0;
}
