#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<pthread.h>

char client_message[2000];
char server_message[2000];
char buffer[1024];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main()
{
	int serverSocket, newSocket;
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;
	
	//Create the socket. 
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	
	// Configure settings of the server address struct
	// Address family = Internet 
	serverAddr.sin_family = AF_INET;
	
	//Set port number, using htons function to use proper byte order 
	serverAddr.sin_port = htons(9999);
	
	//Set IP address to localhost 
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	//Set all bits of the padding field to 0 
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
	
	//Bind the address struct to the socket 
	bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
	
	//Listen on the socket, with 50 max connection requests queued 
	if(listen(serverSocket,50)==0)
		printf("Listening\n");
	else
		printf("Error\n");

	int i = 0;
	while(1)
	{
		//Accept call creates a new socket for the incoming connection
		addr_size = sizeof serverStorage;
		newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);
        

		while(1)
		{
			if (fgets(server_message, 2000 , stdin) != NULL) //reading from stdin
			{
				if (send(newSocket,server_message,strlen(server_message),0) == -1)
					printf("error in sending");
			}
			else
				printf("error in reading from stdin");
			
			if (recv(newSocket,client_message,2000,0) == 0)
				printf("Error");
			else
				fputs(client_message,stdout);
		}
		
		
    }
  return 0;
}