#include <string.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>

#define THREADS 1
using namespace std;

int listenFd;

void *listenBroadcast(void *dummyPtr);

int main(int argc, char* argv[]){

  int portNum;
  bool loop = false;
  struct sockaddr_in serverAddr;
  struct hostent *server;
  pthread_t threads[THREADS];

  //assume all the arguments are valid for now.
  //TODO add invalid arguments checking

  portNum = atoi(argv[2]);

  //create socket
  listenFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


  if(listenFd < 0){
    cerr << "cannot open socket" << endl;
    return 0;
  }

  server = gethostbyname(argv[1]);

  if(server == NULL){
    cerr << "host does not exist" << endl;
    return 0;
  }

  //zero all the memory locations contained by the serverAddr
  bzero((char *) &serverAddr, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  //ipv4 address family, im guessing


  //bcopy is used to copy byte sequences, and is equivalent to the more standard
  //memmove.
  //TODO replace the old, non portable bzero and bcopies to memset and memmoves

  bcopy((char *) server -> h_addr, (char *) &serverAddr.sin_addr.s_addr, server -> h_length);

  serverAddr.sin_port = htons(portNum);

  int checker = connect(listenFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  if(checker < 0){
    cerr << "cannot connect" << endl;
    return 0;
  }

  pthread_create(&threads[0], NULL, listenBroadcast, NULL);
  //sending things to the server

  for(;;){
    char s[300];
    char test[300];
    bzero(test, 301);
    cout << "enter your query here";
    bzero(s, 301);

    cin.getline(s, 300);
    write(listenFd, s, strlen(s));
  }

  pthread_join(threads[0], NULL);

}

void *listenBroadcast(void *dummyPtr){
  while(1){
    char test[300];
    bzero(test, 301);
    read(listenFd, test, 301);
    string tester(test);
    cout << tester << endl;
  }
}
