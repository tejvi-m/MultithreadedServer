#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>

#define THREADS 10

using namespace std;

void *task(void *);

static int connFd;

int main(int argc, char* argv[]){

  int pID, portNum, listenFd;
  socklen_t len;
  bool loop = false;
  struct sockaddr_in serverAddr, clientAddr;

  pthread_t threads[THREADS];

  //TODO check for invalid arguments

  portNum = atoi(argv[1]);

  listenFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if(listenFd < 0){
    cerr << "cannot open socket" << endl;
    return 0;
  }

  bzero((char *) &serverAddr, sizeof(serverAddr));

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(portNum);

  //bind socket

  if(bind(listenFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0){
    cerr << "cannot bind" << endl;
    return 0;
  }

  listen(listenFd, 5);

  int noThread = 0;

  while(noThread < THREADS){

    socklen_t len = sizeof(clntAdd);

    cout << "listening" << endl;

    connFd = accept(listenFd, (struct sockaddr *) &clientAddr, &len);

    if(connFd < 0){
      cout << "cannot accept connection" << endl;
      return 0;
    }

    else{
      cout << "connection successful" << endl;
    }

    pthread_create(&threads[noThread], NULL, task, NULL);

    noThread++;
  }

  for(int i = 0; i < THREADS; i++){
    pthread_join(threads[i], NULL);
  }
}

void *task (void *dummyPtr){
  cout << "Thread number : " << pthread_self() << endl;
  char test[300];
  bzero(test, 301);
  bool loop = false;

  while(!loop){
    bzero(test, 301);

    read(connFd, test, 301);
    string tester(test);
    cout << tester << endl;

    if(tester == "exit"){
      break;
    }

  }

  cout << "closing thread and connection" << endl;

  close(connFd);
}
