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
#include <vector>
#include <pthread.h>

#define THREADS 10

using namespace std;

vector<int> connections;

void broadcast(char* message);
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

    socklen_t len = sizeof(clientAddr);

    cout << "listening" << endl;

    connFd = accept(listenFd, (struct sockaddr *) &clientAddr, &len);
    connections.push_back(connFd);

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

    write(connFd, "got it", strlen("got it"));
    broadcast(test);
    if(tester == "exit"){
      break;
    }

  }

  cout << "closing thread and connection" << endl;

  close(connFd);
}

void broadcast(char * message){

  for(int connection = 0; connection < connections.size(); connection++){
    write(connections[connection], message, strlen(message));
  }
}
