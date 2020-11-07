#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//
#include <sys/types.h>
#include <sys/socket.h>
//
#include <netinet/in.h>
//
#include <arpa/inet.h>

#define TERMINATE 101
#define BUFF_SIZE 255

int main(int argc, char *argv[]) {
  int sockfd, rcvBytes, sendBytes;
  struct sockaddr_in servaddr;

  int SERV_PORT;
  char SERV_ADDR[255];

  socklen_t len;
  char sendBuff[BUFF_SIZE + 1], recvBuff[BUFF_SIZE + 1];

  SERV_PORT = atoi(argv[2]);
  strcpy(SERV_ADDR, argv[1]);

  //Step 1: Construct socket
  printf("%s\n", "Constructing soket...");
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    perror("Error: Problem in creating the socket");
    exit(2);
  }

  //Step 2: Create the remote server socket info structure
  printf("%s\n", "Creating remote server socket info structure...");
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(SERV_ADDR); 
  servaddr.sin_port = htons(SERV_PORT);// convert to big-edian order

  // connect to server socket
  printf("%s\n", "Connecting the the server socket...");
  if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
    printf("\n Error : Connecting to the server failed \n");
    exit(3);
  }

  printf("Connected to the server successfully\n");

  //Step 3: Communicate with server
  char service[1];
  char userName[255];
  char password[255];
  char xxx[255];

  int terminate = 0;
  int logedIn = 0;

  do{
    printf("User name: ");
    scanf("%s", userName);
    if(userName[0] == '\0')
      terminate = 1;
    getchar();
    printf("Send to server: %s\n", userName);
    strcpy(sendBuff, userName);

    send(sockfd, sendBuff, sizeof(sendBuff), 0);

    if((rcvBytes = recv(sockfd, recvBuff, sizeof(recvBuff), 0)) == 0) {
      perror("The server terminated prematurely");
      exit(4);
    } else {
      recvBuff[rcvBytes] = '\0';
      printf("%s\n", recvBuff);
    }
  }while(terminate == 0);

  // close the descriptor
  close(sockfd);
}
