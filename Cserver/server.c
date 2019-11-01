#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h> 	
#include <dirent.h>
#include <pthread.h>

#define LIST "list"
#define GET "get"

char* ROOT;
pthread_mutex_t lock;//in case of multiple clients accessing a file at the same time

void* clientAction(void* arg){

    pthread_mutex_lock(&lock);
	char data_to_send[1024];
	char client_message[2048];
	int client_fd = *((int *) arg);
	
	recv(client_fd, client_message , 2048, 0);
	//client_message, which is a request, should be processed accoding to application, here maybe for a file.//To be done later.
	int bytes_read;
	char* path = (char*)malloc(sizeof(*ROOT));
	strcpy(path, ROOT);
	// char file[] = "/a.txt";

	strcat(path, client_message);
    printf("%s\n", path);
	// /home/thejas/MultithreadedServer/a.txt
	int fd = open(path, O_RDONLY);
	if (fd!=-1){
		
		while((bytes_read=read(fd, data_to_send, 1024))>0)
			send(client_fd, data_to_send, bytes_read, 0);

	}

	printf("exiting client action\n");
	close(client_fd);
    pthread_mutex_unlock(&lock);

}




int main()
{
	int sockfd = 0;				        // listen - listen file descriptor
	int connfd = 0;					// connect - connect file descriptor
	char recvBuff[256];				
	struct sockaddr_in serv_addr;			// A sockaddr_in is a structure containing an internet address. 
							// This structure is defined in <netinet/in.h>.
	char sendBuff[1024];
	int numrv;
	char filename[1024];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);	// listen - listen for connections on a socket
							// AF_INET refers to the address family ipv4. 
							// The SOCK_STREAM means connection oriented TCP protocol.

	printf("Socket retrieve success\n");

  	memset(&serv_addr, '0', sizeof(serv_addr));	// memset() is used to fill a block of memory with a particular value.
	memset(sendBuff, '0', sizeof(sendBuff));

	serv_addr.sin_family = AF_INET;			// A sockaddr_in is a structure containing an internet address. 
							// This structure is defined in <netinet/in.h>.

	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	
	serv_addr.sin_port = htons(5054);		// port:5050

	bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));	//        bind - bind a name to a socket


	if (listen(sockfd, 10) == -1)			// listen - listen for connections on a socket
  	{
		printf("Failed to listen\n");
		return -1;
  	}

	pthread_t tid[60]; //handle 60 clients as of now.
	int client_accepted;
	int i = 0;
	while (1)
  	{
		ROOT = getenv("PWD");
		client_accepted = accept(sockfd, (struct sockaddr *)NULL, NULL);
		if(client_accepted < 0){
			printf("error");
			return -1;
		}
		else{
			//pthread_create(&tid[i], NULL, socketThread, &newSocket)
            if (pthread_mutex_init(&lock, NULL) != 0) { 
                printf("mutex init failed\n"); 
                return 1; 
            } 
			if(pthread_create(&tid[i], NULL, clientAction, &client_accepted) != 0){
				printf("failed");

			}
			else{
				pthread_join(tid[i++],NULL);			
			
			}
		}
		if(i >= 60){
			printf("resetting\n");
			i = 0;
		}

		// TODO:  Read if the client wants a GET or a LIST using a read() systemcall.

		// If client sends a GET

		
  	}
    
    pthread_mutex_destroy(&lock);
	return 0;
	
}

