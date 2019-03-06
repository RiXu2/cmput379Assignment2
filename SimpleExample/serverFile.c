#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<pthread.h>
#include <dirent.h>
#include <sys/stat.h>

char client_message[2000];
char server_message[2000];
char buffer[1024];

int cfileexists(const char * filename){
    /* try to open file to read */
    FILE *file;
	printf("%s", filename);
    if (file = fopen(filename, "r")){
		printf("file is here\n");
        fclose(file);
        return 1;
    }
    return 0;
}


void * socketThread(void *arg)
{
	struct stat stat_buf;
	int read_fd;
	int newSocket = *((int *)arg);
	// Send message to the client socket 
	
	while(1)
	{
		char str[80];
		if(recv(newSocket,client_message,2000,0)==0)
            printf("Error");
        else
		{
			//removing \n from the recieved data
			client_message[strcspn(client_message, "\n")] = 0;
			
			strcpy(str, "./");
			strcpy(str, client_message);
			fputs(client_message,stdout);
			printf("\nThis is the first message\n");
			if( cfileexists(str) == 1 )
			{
				char * message = "sending the file";
				send(newSocket,message,strlen(message),0);
				read_fd = open (str, O_RDONLY);
				fstat (read_fd, &stat_buf);
				printf("%d\n", stat_buf.st_size);
				sendfile(newSocket, read_fd, 0, stat_buf.st_size);
			}
			else
			{
				printf("we do not have it\n");
			}
		}
		fgets(server_message, 2000 , stdin);
		send(newSocket,server_message,strlen(server_message),0);
	}
	
	send(newSocket,buffer,13,0);
	printf("Exit socketThread \n");
	close(newSocket);
	pthread_exit(NULL);
	
}


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
	
	//Listen on the socket, with 40 max connection requests queued 
	if(listen(serverSocket,50)==0)
		printf("Listening\n");
	else
		printf("Error\n");
	pthread_t tid[60];
	int i = 0;
	while(1)
	{
		//Accept call creates a new socket for the incoming connection
		addr_size = sizeof serverStorage;
		newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);
        
		//for each client request creates a thread and assign the client request to it to process
        //so the main thread can entertain next request
		if( newSocket >= 0)
		{
			printf("%d", i);
			printf("I am making sockets\n");
			if( pthread_create(&tid[i], NULL, socketThread, &newSocket) != 0 )
				printf("Failed to create thread\n");
				//printf("%lu\n", tid[i]);
			i++;
		}
		else if( newSocket < 0)
			printf("Failed to connect\n");
			
        if( i >= 50)
        {
			i = 0;
			while(i < 50)
			{
				pthread_join(tid[i++],NULL);
			}
			i = 0;
        }
    }
  return 0;
}