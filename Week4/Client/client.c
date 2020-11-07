#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define TERMINATE 101

#define BUFF_SIZE 255

int main(int argc, char *argv[]) {
  int SERV_PORT = atoi(argv[2]);
  char SERV_ADDR[255];
  strcpy(SERV_ADDR, argv[1]);

  int sockfd, rcvBytes, sendBytes;
  socklen_t len;
  char buff[BUFF_SIZE+1];
  struct sockaddr_in servaddr;

  //Step 1: Construct socket
  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("Error: "); 
    return 0; 
  }

  //Step 2: Define the address of the server
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(SERV_ADDR); 
  servaddr.sin_port = htons(SERV_PORT);


  // The kernel just checks for any immediate errors and returns immediately to the calling process
  // connect to server
  if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    { 
      printf("\n Error : Connect Failed \n");
      exit(0);
    } 

  //Step 3: Communicate with server

  char service[1];
  char userName[255];
  char password[255];
  char xxx[255];

  int terminate = 0;
  int logedIn = 0;

  do{
    do{
      scanf("%s", userName);
      if(userName[0] == '\0') terminate = 1;
      getchar();
      printf("Insert password: ");
      scanf("%s", password);
      getchar();
      if(password[0] == '\0') terminate = 1;

      service[0]='3';
      service[1]='\n';
      len = sizeof(servaddr);
      sendBytes = sendto(sockfd,service , sizeof(service), 0, (struct sockaddr *) &servaddr, len);
      if(sendBytes < 0){
	perror("Error: ");
	return 0;
      }

      len = sizeof(servaddr);
      sendBytes = sendto(sockfd, userName, sizeof(userName), 0, (struct sockaddr *) &servaddr, len);
      if(sendBytes < 0){
	perror("Error: ");
	return 0;
      }

      len = sizeof(servaddr);
      sendBytes = sendto(sockfd, password, sizeof(password), 0, (struct sockaddr *) &servaddr, len);
      if(sendBytes < 0){
	perror("Error: ");
	return 0;
      }


      char logInRes[255];
      do{
	rcvBytes = recvfrom(sockfd, logInRes, sizeof(logInRes), 0, (struct sockaddr *) &servaddr, &len);

	if(rcvBytes < 0){
	  perror("Error: ");
	  return 0;
	}
    
	logInRes[rcvBytes] = '\0';

// if OK thif duwng


	printf("%s", logInRes);
      }while(logInRes[0] != '\n');
    }while(logedIn == 0);

do {
	service[0]='7';
	service[1]='\n';
	len = sizeof(servaddr);
	sendBytes = sendto(sockfd,service , sizeof(service), 0, (struct sockaddr *) &servaddr, len);

	if(sendBytes < 0){
	  perror("Error: ");
	  return 0;
	}


	scanf("%s", xxx);
	if(xxx[0] == '\0') terminate = 1;
	getchar();

	len = sizeof(servaddr);
	sendBytes = sendto(sockfd, xxx, sizeof(xxx), 0, (struct sockaddr *) &servaddr, len);
	if(sendBytes < 0){
	  perror("Error: ");
	  return 0;
	}

	rcvBytes = recvfrom(sockfd, xxx, sizeof(xxx), 0, (struct sockaddr *) &servaddr, &len);

	if(rcvBytes < 0){
	  perror("Error: ");
	  return 0;
	}
    
	xxx[rcvBytes] = '\0';

	printf("%s", xxx);
      }while(terminate == 0);

  }while(terminate == 0);

  // close the descriptor
  close(sockfd);
}
