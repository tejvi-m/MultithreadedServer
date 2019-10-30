#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define LIST "list"
#define GET "get"

void* clientThread(void* arg){
 int sockfd = 0;					// sockfd - socket file descriptor
	int bytesReceived = 0;				// Bytes received from server
	char recvBuff[56000];				// Buffer to receive bytes from server

	memset(recvBuff, '0', sizeof(recvBuff));	// memset() is used to fill a block of memory with a particular value.

	struct sockaddr_in serv_addr;			// A sockaddr_in is a structure containing an internet address. 
							// This structure is defined in <netinet/in.h>.

		  /* Create a socket first */

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 		// Create socket
	{
		printf("\n Error : Could not create socket \n");
	    exit(0);
  	}


	  /* Initialize sockaddr_in data structure */

	serv_addr.sin_family = AF_INET;				
	serv_addr.sin_port = htons(5050);  			// port:5050
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");	// local host IP address: 127.0.0.1


	  /* Attempt a connection */

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) // connect - initiate a connection on a socket.
	{
		printf("\n Error : Connect Failed \n");
		exit(0);
	}

    // TODO:  Send GET to the server using the "sockfd" file descriptor
    //puts("sending get request\n");
    // TODO:  Send the filename to the server using the "sockfd" file descriptor
    
   char* f_name = (char *) arg;
    
    write(sockfd, f_name, 256);

    int totalBytes = 0;	
    while ((bytesReceived = read(sockfd, recvBuff, 256)) > 0)
    {
        // TODO:  Use the write() system call to write those contents in the file which was created above.
        //write(file, recvBuff, bytesReceived);
        //printf("%d\n",bytesReceived);
        //memset(recvBuff, '0', sizeof(recvBuff));
        totalBytes += bytesReceived;
    }
    printf("Bytes received:%d\n",totalBytes);
    printf("%s\n", recvBuff);
}

int main() 
{
    int i = 0;
    pthread_t tid[61];


    while(1){
        char f_name[256];
        printf("which file\n");
        scanf("%s", f_name);

        if( pthread_create(&tid[i], NULL, clientThread, f_name) != 0 )
                printf("Failed to create thread\n");
        else
            pthread_join(tid[i++], NULL);
        if(i > 60){
            printf("resetting\n");
            i = 0;
        }

    }


/*     while(i< 60)
    {
        printf("in creation\n");
        if( pthread_create(&tid[i], NULL, cientThread, NULL) != 0 )
                printf("Failed to create thread\n");

        i++;
    }

    // sleep(20);
    i = 0;

    while(i< 60)
    {

        pthread_join(tid[i++],NULL);
        printf("%d:\n",i);

    }
 */
	return 0;
}