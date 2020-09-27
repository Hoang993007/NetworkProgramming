#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Internally, the system uses a database to keep track of the mapping between host names and host numbers.
#include <unistd.h>

// The functions and other symbols for accessing this database are declared in netdb.h.
#include <netdb.h>

// If the name lookup by gethostbyname or gethostbyaddr fails, you can find out the reason by looking at the value of the variable h_errno.
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>

// The data types for representing socket addresses in the Internet namespace are defined in the header file netinet/in.h.
#include <netinet/in.h>
#include <arpa/inet.h>

#define IPFormat AF_INET // AF_INET for IPv4 and AF_INET6 for IPv6

void convertIPAddrToDomainName (char* IPAddr);
void convertDomainNameToIPAddr (char* domainName);

int main(int argc, char *argv[]) {
  int option = atoi(argv[1]);

  switch (option) {
  case 1:
    // argv[2] is IP address
    convertIPAddrToDomainName(argv[2]);
    break;

  case 2:
    // argv[2] is domain name
    convertDomainNameToIPAddr(argv[2]);
    break;
  }

  return 0;
}

void invalidAddressErr (int rval) {
  if (rval == 0 || rval == -1) {
    printf("Not found information");
    printf("\n");
    exit(EXIT_FAILURE);
  }
}

void checkHostName (struct hostent * host) {
  if(h_errno  == HOST_NOT_FOUND) {
    printf ("Not found information");
    printf("\n");
    exit(EXIT_FAILURE);
  }
}

void convertIPAddrToDomainName (char* IPAddr) {
  for(int i = 0; i < strlen(IPAddr); i++) {
    char c = IPAddr[i];
    if(c != '.' && (c > '9' || c < '0')) {
      printf ("Wrong parameter");
      printf("\n");
      exit(EXIT_FAILURE);
    }
  }

  struct hostent* host = NULL;
  int rval;

  if(IPFormat == AF_INET) {
    struct in_addr inaddr;
    rval = inet_pton(AF_INET,IPAddr, &inaddr);

    invalidAddressErr(rval);

    host = gethostbyaddr(&inaddr, sizeof(inaddr), IPFormat);
  } else if(IPFormat == AF_INET6) {
    struct in6_addr in6addr;
    rval = inet_pton(AF_INET6, IPAddr, &in6addr);

    invalidAddressErr(rval);

    host = gethostbyaddr(&in6addr, sizeof(in6addr), IPFormat);
  }

  checkHostName(host);

  // get official name
  printf ("Official name: %s", host->h_name);
  printf("\n");

  // get alias name
  printf ("Alias name:");
  printf ("\n");
  for(int i = 0; host->h_aliases[i] != NULL; i++) {
    printf ("%s", host->h_aliases[i]);
    printf("\n");
  }
}

void convertDomainNameToIPAddr (char* domainName) {
  for(int i = 0; i < strlen(domainName); i++) {
    char c = domainName[i];
    if(c != '.' && (c > '9' || c < '0')) {
      break;
    }
    if(i == strlen(domainName) - 1) {
      printf ("Wrong parameter");
      printf("\n");
      exit(EXIT_FAILURE);
    }
  }

  struct hostent* host;
  host = gethostbyname2 (domainName, IPFormat);

  checkHostName(host);

  char IPBuf[35]; // For both IPv4 and IPv6
  // Converts an IP address into an ASCII string representing the address in dot notationget official name

  // get official IP
  printf ("Official IP: ");

  if(inet_ntop(host->h_addrtype, host->h_addr, IPBuf, sizeof(IPBuf)) != NULL) {
    printf ("%s", IPBuf);
  }
  printf("\n");

  // get alias IP
  printf ("Alias IP: ");
  printf("\n");

  for(int i = 1; host->h_addr_list[i] != NULL; i++) {
    printf ("%s", inet_ntop(host->h_addrtype, host->h_addr_list[i], IPBuf, 35));
    printf("\n");
  }
}
