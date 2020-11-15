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

#define RECV_BUFF_SIZE 4096
#define LISTENQ 10 /* maximum number of client connection */

void tostring(char [], int);
int toint(char []);
void sig_chld(int signo);

int main(int argc, char *argv[]) {
  init("./Server/bin/nguoidung.txt");

  if(argc != 2) {
    printf("Parameter incorrect\n");
    exit(0);
  }

  int SERV_PORT;
  int listenfd, connfd, recvBytes;

  int connfdArr[10];
  int connfCount = 0;

  // pid_t data type stands for process identification and is used to represent process ids.
  pid_t childpid;
  socklen_t clientSocketLen; // size of client socket address

  struct sockaddr_in servaddr, cliaddr; // address structure

  char recvBuff[RECV_BUFF_SIZE + 1];

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

  signal(SIGCHLD, sig_chld);

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
      connfdArr[connfCount] = connfd;
      connfCount++;
    }

    if((childpid = fork()) == 0) {// if true => child process is successfully created
      // child process id are stored in childpid

      //close listening socket in child process
      // there for now only parent process will listen to new connection and create a new fork, not child process
      close (listenfd);

      printf("%s\n", "New child process are created by copping the parent process");

      loadDataFromFile();
      printf("Database are now ready\n");

      while((recvBytes = recv(connfd, recvBuff, sizeof(recvBuff), 0)) > 0) {// receive service number
	recvBuff[recvBytes] = '\0';
	printf("[%s:%d]: Service num: %s\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), recvBuff);

        int service;
	service = atoi(recvBuff);

	printDB();

	switch(service){
	case 1:
	  printf ("%s\n", "Service 1:");

	  userNameType userName;
	  passwordType password;

	  //accountRegister();
	  break;

	case 2:
	  printf ("%s\n", "Service 2:");

	  //activate();
	  break;

	case 3:
	  printf ("%s\n", "Service 3:");

	  recvBytes = recv(connfd, recvBuff, sizeof(recvBuff), 0);
	  userName[recvBytes] = '\0';
	  strcpy(userName, recvBuff);
	  printf("[%s:%d]: User name: %s\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), userName);

	  if(isExistUserName(userName) == ACCOUNT_EXIST) {
	    send(connfd, "O", sizeof("O"), 0);
	  } else {
	    printf("Account doesn't exit\n");
	    send(connfd, "X", sizeof("X"), 0);
	    break;
	  }

	  recvBytes = recv(connfd, recvBuff, sizeof(recvBuff), 0);
	  recvBuff[recvBytes] = '\0';
	  strcpy(password, recvBuff);
	  printf("[%s:%d]: password: %s\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), password);

	  if(isLogedIn(inet_ntoa(cliaddr.sin_addr)) == NOT_LOGED_IN) {
	    int res = logIn (inet_ntoa(cliaddr.sin_addr), userName, password);
	    // for test
	    // printf("%d\n", res);
	    char sres[10];
	    tostring(sres, res);
	    send(connfd, sres, sizeof(sres), 0);
	  }
	  break;


	case 4:
	  if(isLogedIn(inet_ntoa(cliaddr.sin_addr)) == LOGED_IN)
	    // search()
	    ;
	  else printf("Not loged in\n");
	  break;

	case 6:
	  printf ("%s\n", "Service 6:");

	  if(isLogedIn(inet_ntoa(cliaddr.sin_addr)) != LOGED_IN) {
	    printf("Not loged in\n");
	  } else {
	    passwordType newPassword;
	    printf("check1!!!\n");
	    recvBytes = recv(connfd, recvBuff, sizeof(recvBuff), 0);
	    puts(recvBuff);
	      printf("check2!!!\n");
	    recvBuff[recvBytes] = '\0';
	    strcpy(newPassword, recvBuff);
	    printf("New password: %s\n", newPassword);
	    int numChar = 0;

	    for(int i = 0; i < strlen(newPassword); i++) {
	      if (newPassword[i] < '0'
		  || (newPassword[i] > '9' && newPassword[i] < 'A')
		  || (newPassword[i] > 'Z' && newPassword[i] < 'a')
		  || newPassword[i] > 'z') {
		send(connfd, "Error", sizeof("Error"), 0);
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
	    send(connfd, encodePass, sizeof(encodePass), 0);
	  }
	  break;

	case 7:
	  if(isLogedIn(inet_ntoa(cliaddr.sin_addr)) == LOGED_IN) {
	    signOut(inet_ntoa(cliaddr.sin_addr));
	    printDB();
	    perror("Client exiting\n");

	    close(connfd);// Child process close connection to the client
	    exit(0); // end child process
	  }
	  else printf("Not loged in\n");
	  break;
	}
      }
    }

    if(recvBytes < 0) {
      perror("Client exited\n");
      close(connfd); // Child process close connection to the client
      exit(0); // end child process
    }
  }

  printf("Closing the file descriptor of the server...\n");
  close(connfd);

  rubbishCollection();
  freeLogIn();
  //another rubbish collection

  return 0;
}

// -----------------------------------------------------------------------
void sig_chld(int signo) {
  pid_t pid;
  int stat;
  pid = waitpid(-1, &stat, WNOHANG );
  loadDataFromFile();
  printf("%d\n", SIGCHLD);
  printf("child %d terminated\n", pid);
}

// ------------------------------------------------------------------------
void tostring(char str[], int num)
{
  int i, rem, len = 0, n;

  n = num;
  while (n != 0)
    {
      len++;
      n /= 10;
    }
  for (i = 0; i < len; i++)
    {
      rem = num % 10;
      num = num / 10;
      str[len - (i + 1)] = rem + '0';
    }
  str[len] = '\0';
}
