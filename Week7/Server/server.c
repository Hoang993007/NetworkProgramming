#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//
#include <sys/types.h>
#include <sys/socket.h>
//
#include <netinet/in.h>
//
#include <unistd.h>
//
#include <arpa/inet.h> 
#include <errno.h>
//
#include "./inc/services.h"
#include "./inc/accountSystem.h"

#define RECV_BUFF_SIZE 4096
#define SEND_BUFF_SIXE 4096
#define LISTENQ 3 /* maximum number of client connection */

int main(int argc, char *argv[]) {
  init("./Server/bin/nguoidung.txt");

  if(argc != 2) {
    printf("Parameter incorrect\n");
    exit(0);
  }

  int SERV_PORT;
  int listenfd, connfd, recvBytes;
  // pid_t data type stands for process identification and is used to represent process ids.
  pid_t childpid;
  socklen_t clientSocketLen; // size of client socket address

  struct sockaddr_in servaddr, cliaddr; // address structure

  char recvBuff[RECV_BUFF_SIZE + 1], sendBuff[SEND_BUFF_SIZE + 1];

  SERV_PORT = atoi(argv[1]);

  bzero(&servaddr, sizeof(servaddr));

  //Step 1: Construct socket
  printf("Constructing socket IPv4 - TCP...\n");

  if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    perror("Error: Error in creating socket");
    exit(2);
  }

  // Preparation of the server address
  servaddr.sin_family = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY); /* kernel chooses IP address for server - this time is 172.0.0.1 - local host address */
  servaddr.sin_port = htons(SERV_PORT); // specifies port

  //Step 2: Bind address to socket
  printf("Binding servaddr address to the socket...\n");

  if(bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr))){
    perror("Error: ");
    exit(EXIT_FAILURE);
  }

  printf("Server started.\n");

  // Establish a socket to LISTENING for incoming connection
  // Listen to the socket by creating a connection queue, then wait for clients
  printf("Establishing a socket to LISTENING for incoming connection\n");
  if(listen(listenfd, LISTENQ) < 0){
    perror("Error: Error in establising socket to listen for incoming connection");
    exit(EXIT_FAILURE);
  }

  printf("%s\n", "Server are now waiting for connections...");

  //Step 3: Communicate with client
  /* now loop, receiving data and printing what we received */

  for(;;){
    clientSocketLen = sizeof(cliaddr);

    // Accept a connection request -> return a File Descriptor (FD)
    if((connfd = accept(listenfd, (struct sockaddr*) &cliaddr, &clientSocketLen)) < 0){
      perror("Error: ");
      exit(EXIT_FAILURE);
    } else {
      printf("%s\n", "Accepted  connection request...");
    }

    if((childpid = fork()) == 0) {// if true => child process
      printf("%s\n", "Child created for dealing with client request");

      //close listening socket
      close (listenfd);

      while((recvBytes = recv(connfd, recvBuff, sizeof(recvBuff), 0)) > 0) {// receive service number
	recvBuff[recvBytes] = '\0';
	printf("[%s:%d]:\n\t%s\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), recvBuff);

        int service;
	service = atoi(buff);

	switch(service){
	case 1:
	  printf ("%s\n", "Service 1:");

	  char userName[255];
	  char password[255];

	  //accountRegister();
	  break;

	case 2:
	  printf ("%s\n", "Service 2:");

	  //activate();
	  break;

	case 3:
	  printf ("%s\n", "Service 3:");

	  recv(connfd, userName, sizeof(userName), 0);
	  recv(connfd, password, sizeof(password), 0);

	  if(isLogedIn(inet_ntoa(cliaddr.sin_addr)) == NOT_LOGED_IN) {
	    int res = logIn (inet_ntoa(cliaddr.sin_addr), userName, password);
	    // for test
	    // printf("%d\n", res);

	    if(res == LOGIN_SUCCESS){
	      strcpy(sendBuff, "OK\n");
	      send(connfd, sendBuff, sizeof(sendBuff), 0);
	    }
	    else if(res == ACCOUNT_BLOCKED) {
	      strcpy(sendBuff, "Account is blocked\n");
	      send(connfd, sendBuff, sizeof(sendBuff), 0);
	    }
	    else if(res == ACCOUNT_IDLE) {
	      strcpy(sendBuff, "Account not ready\n");
	      send(connfd, sendBuff, sizeof(sendBuff), 0);
	    }
	    else {// wrong password
	      strcpy(sendBuff, "NOT OK\n");
	      send(connfd, sendBuff, sizeof(sendBuff), 0);
	    }
	  }
	  else {
	    printf("Already loged in\n");
	  }
	  break;


	case 4:
	  if(isLogedIn(inet_ntoa(cliaddr.sin_addr)) == LOGED_IN)
	    // search();
	  else printf("Not loged in\n");
	  break;

	case 5:
	  if(isLogedIn(inet_ntoa(cliaddr.sin_addr)) == LOGED_IN)
	    // changePass(IP);
	  else printf("Not loged in\n");
	  break;

	case 6:
	  if(isLogedIn(inet_ntoa(cliaddr.sin_addr)) == LOGED_IN)
	    // signOut(IP);
	  else printf("Not loged in\n");
	  break;

	case 7:
	  recv(connfd, recvBuff, sizeof(recvBuff), 0);
	  strcpy(sendBuff, recvBuff);
	  send(connfd, sendBuff, sizeof(sendBuff), 0);
	  send(connfd, "\n", sizeof("\n"), 0);
	  break;
	}
      }
    }

    if(rcvBytes < 0) {
      perror("%s\n", "Read error");
      exit(0);
    }
  }

  printf("Closing the file descriptor of the server...\n");
  close(connfd);

  rubbishCollection();
  //another rubbish collection

  return 0;
}
