#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
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
#include "./inc/errorCode.h"
#include "./inc/convertNumAndStr.h"
//
#include <sys/select.h>
#include <sys/time.h>

#define RECV_BUFF_SIZE 4096
#define LISTENQ 3 /* the number of pending connections that can be queued for a server socket. (call waiting allowance */

#define MAX_CLIENT 3

void service_register(struct sockaddr_in cliaddr, int clientConnfd);
void service_activate(struct sockaddr_in cliaddr, int clientConnfd);
void service_signin(struct sockaddr_in cliaddr, int clientConnfd);
void service_changePass(struct sockaddr_in cliaddr, int clientConnfd);
void service_newGame(struct sockaddr_in cliaddr, int clientConnfd);
void service_gamePlayingHistory(struct sockaddr_in cliaddr, int clientConnfd);
void service_signout(struct sockaddr_in cliaddr, int clientConnfd);

int main(int argc, char *argv[]) {
  loadUsername_passData();
  printf("Server is now ready for logining\n");

  if(argc != 2) {
    printf("Parameter incorrect\n");
    exit(0);
  }

  int SERV_PORT;
  int listenfd, connfd, recvBytes;

  // clientConnfd hold ID of client connection
  int clientConnfd[FD_SETSIZE], askForCheckingActivity;

  int maxfd;

  //set of socket descriptors
  fd_set readfds;

  struct timeval tv;

  socklen_t clientSocketLen; // size of client socket address

  struct sockaddr_in servaddr, cliaddr; // address structure

  char recvBuff[RECV_BUFF_SIZE + 1];


  SERV_PORT = atoi(argv[1]);

  bzero(&servaddr, sizeof(servaddr));

  //Step 1: Construct socket
  printf("Constructing socket IPv4 - TCP...\n");

  if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    // listenfd is the socketID
    perror("Error: Error in creating socket");
    exit(2);
  }

  // Preparation of the server address
  servaddr.sin_family = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  /* INADDR_ANY: kernel chooses IP address for server
     This time is 172.0.0.1 - local host address */
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

  // Assign initial value for the array of connection socket
  for(int i = 0; i < MAX_CLIENT; i++) {
    clientConnfd[i] = -1;
  }

  /* clear all bits in fdset */
  FD_ZERO(&readfds);

  // fd_sets use to specify the descriptors that we want the kernel to test for reading, writing, and exception conditions.

  // add listenfd to set and turn on the bit for fd in set
  FD_SET(listenfd, &readfds);
  maxfd = listenfd;

  // maxfd is the highest socket ID in readfds

  printf("%s\n", "Server are now waiting for connections...");

  //Step 3: Communicate with client
  /* now loop, receiving data and printing what we received */

  while(1){
    clientSocketLen = sizeof(cliaddr);

    //wait for an activity from a descriptor
    //(timeout 10.5 secs)
    tv.tv_sec = 10;
    tv.tv_usec = 500000;

    // The select() function asks kernel to simultaneously check multiple sockets (saved in readfs)...
    //...to see if they have data waiting to be recv(), or if you can send() data to them without blocking, or if some exception has occurred.
    askForCheckingActivity = select(maxfd + 1, &readfds, NULL, NULL, &tv);
    //  On success, returns the total number of bits that are set(that is the number of ready file descriptors)
    // • On time-out, returns 0
    // • On error, return -1

    if(askForCheckingActivity == -1) {
      perror("\Error: ");
      // error occurred in select()
    } else if(askForCheckingActivity == 0) {
      printf("Timeout occurred! No data after 10.5s \n");
    } else printf ("The number of ready file descriptors: %d\n", askForCheckingActivity);


    // NOTE: the listenfd or connfd are both descriptor
    // select() check if a descriptor have an activity to exec or not. Not only use for listenfd or connfd

    // NOTE: in this small program we use only listenfd socket to connect with client

    // check the status of listenfd
    // if there is a client want to connect through listenfd socket the select() function will mark listenfd in readfds as 1 - have an activity
    if (FD_ISSET (listenfd, &readfds)) {
      // Accept a connection request -> return a File Descriptor (FD)
      if((connfd = accept(listenfd, (struct sockaddr*) &cliaddr, &clientSocketLen)) < 0){
	perror("Error: ");
	exit(EXIT_FAILURE);

      } else {
	printf("New connection, socket fd is %d, ip is: %s, prot: %d\n", connfd, inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

	for(int i = 0; i < MAX_CLIENT; i++) {
	  if(clientConnfd[i] == -1) {
	    clientConnfd[i] = connfd;

	    break;
	  }
	}

	FD_SET(connfd, &readfds);
	if(connfd > maxfd) {
	  maxfd = connfd;
	}
      }
    }

    // check the status of connfd(s)
    for(int k = 0; k < MAX_CLIENT; k++) {
      if(clientConnfd[k] != -1) {
	// printf("client connection fd: %d\n", clientConnfd[k]);

	if(FD_ISSET(clientConnfd[k], &readfds)) {
	  // printf("Do something\n");

	  while((recvBytes = recv(clientConnfd[k], recvBuff, sizeof(recvBuff), 0)) > 0) {// receive service number
	    recvBuff[recvBytes] = '\0';
	    printf("[%s:%d]: Service num: %s\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), recvBuff);

	    int service;
	    service = atoi(recvBuff);

	    print_username_pass();

	    switch(service){
	    case 1:
	      printf ("%s\n", "Service 1: Register");
              service_register(cliaddr, clientConnfd[k]);
	      break;

	    case 2:
	      printf ("%s\n", "Service 2: Activate account");
              service_activate(cliaddr, clientConnfd[k]);
	      break;

	    case 3:
	      printf ("%s\n", "Service 3: Sign in");
              service_signin(cliaddr, clientConnfd[k]);
	      break;

	    case 4:
	      printf ("%s\n", "Service 4: Change password");
	      if(isLogedIn(inet_ntoa(cliaddr.sin_addr)) == LOGED_IN) {
              	service_changePass(cliaddr, clientConnfd[k]);
              }
	      else printf("Not loged in\n");
	      break;

	    case 5:
	      printf ("%s\n", "Service 5: New game");
	      if(isLogedIn(inet_ntoa(cliaddr.sin_addr)) == LOGED_IN) {
              	service_newGame(cliaddr, clientConnfd[k]);
              }
	      else printf("Not loged in\n");	      
	      break;

	    case 6:
	      printf ("%s\n", "Service 6: Game playing history");
	      if(isLogedIn(inet_ntoa(cliaddr.sin_addr)) == LOGED_IN) {
              	service_gamePlayingHistory(cliaddr, clientConnfd[k]);
              }
	      else printf("Not loged in\n");
	      break;

	    case 7:
	      printf ("%s\n", "Service 7: Sign out");
	      if(isLogedIn(inet_ntoa(cliaddr.sin_addr)) == LOGED_IN) {
              	service_signout(cliaddr, clientConnfd[k]);
              }
	      else printf("Not loged in\n");
	      break;
	    }

	    if(recvBytes < 0) {
	      perror("Client exited\n");
	    }
	  }

	  printf("Closing the file descriptor of the client connection...\n");

	  FD_CLR(clientConnfd[k], &readfds);
	  close(clientConnfd[k]);
	  clientConnfd[k] = -1;
	}
      }
    }
  }

  freeAccountNode();
  freeLogIn();

  return 0;
}

void service_register(struct sockaddr_in cliaddr, int clientConnfd) {

}

void service_activate(struct sockaddr_in cliaddr, int clientConnfd) {

}

void service_signin(struct sockaddr_in cliaddr, int clientConnfd) {
  int recvBytes;
  char recvBuff[RECV_BUFF_SIZE + 1];

  userNameType userName;
  passwordType password;

  recvBytes = recv(clientConnfd, recvBuff, sizeof(recvBuff), 0);
  userName[recvBytes] = '\0';
  strcpy(userName, recvBuff);
  printf("[%s:%d]: User name: %s\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), userName);

  if(isExistUserName(userName) == ACCOUNT_EXIST) {
    send(clientConnfd, "O", sizeof("O"), 0);
  } else {
    printf("Account doesn't exit\n");
    send(clientConnfd, "X", sizeof("X"), 0);
    return;
  }

  recvBytes = recv(clientConnfd, recvBuff, sizeof(recvBuff), 0);
  recvBuff[recvBytes] = '\0';
  strcpy(password, recvBuff);
  printf("[%s:%d]: password: %s\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), password);

  if(isLogedIn(inet_ntoa(cliaddr.sin_addr)) == NOT_LOGED_IN) {
    int res = logIn (inet_ntoa(cliaddr.sin_addr), userName, password);
    // for test
    // printf("%d\n", res);
    char sres[10];
    tostring(sres, res);
    send(clientConnfd, sres, sizeof(sres), 0);
  } else {
    printf("You're alrealdy logined\n");
  }
}

void service_changePass(struct sockaddr_in cliaddr, int clientConnfd) {
  int recvBytes;
  char recvBuff[RECV_BUFF_SIZE + 1];

  if(isLogedIn(inet_ntoa(cliaddr.sin_addr)) != LOGED_IN) {
    printf("Not loged in\n");
  } else {
    passwordType newPassword;

    recvBytes = recv(clientConnfd, recvBuff, sizeof(recvBuff), 0);
    recvBuff[recvBytes] = '\0';
    strcpy(newPassword, recvBuff);
    printf("New password: %s\n", newPassword);

    int numChar = 0;
    for(int i = 0; i < strlen(newPassword); i++) {
      if (newPassword[i] < '0'
	  || (newPassword[i] > '9' && newPassword[i] < 'A')
	  || (newPassword[i] > 'Z' && newPassword[i] < 'a')
	  || newPassword[i] > 'z') {
	send(clientConnfd, "Error", sizeof("Error"), 0);
	break;
      }

      if(newPassword[i] <= '9' && newPassword[i] >= '0') {
	numChar++;
      }
    }
    printf("Changing password...\n");

    changePass(inet_ntoa(cliaddr.sin_addr), newPassword);

    passwordType encodePass;
    int charCharIndex, numCharIndex;
    encodePass[strlen(newPassword)] = '\0';
    charCharIndex = numChar;
    numCharIndex = 0;
    printf("New password num: %d\n", numChar);
    printf("New password char: %d\n", (int)(strlen(newPassword) - numChar));

    for(int i = 0; i < strlen(newPassword); i++) {
      if(newPassword[i] <= '9' && newPassword[i] >= '0') {
	encodePass[numCharIndex] = newPassword[i];
	numCharIndex++;
      } else {
	encodePass[charCharIndex] = newPassword[i];
	charCharIndex++;
      }
    }
    printf("EncodedPass: %s\n", encodePass);
    send(clientConnfd, encodePass, sizeof(encodePass), 0);
  }
}

void service_newGame(struct sockaddr_in cliaddr, int clientConnfd) {

}

void service_gamePlayingHistory(struct sockaddr_in cliaddr, int clientConnfd) {

}

void service_signout(struct sockaddr_in cliaddr, int clientConnfd) {
  if(isLogedIn(inet_ntoa(cliaddr.sin_addr)) == LOGED_IN) {
    signOut(inet_ntoa(cliaddr.sin_addr));
    print_username_pass();
    printf("Client exited\n");
  }
  else printf("Not loged in\n");
}
