#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_SIZE 50
#define NUM_CLIENT 1
void *connection_handler(void *socket_desc);
int main()
{
    int sock_desc;
	int write_fd;
    struct sockaddr_in serv_addr;
    char sbuff[MAX_SIZE],rbuff[MAX_SIZE], filebuff[MAX_SIZE];
	//off_t offset = 0;

    if((sock_desc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        printf("Failed creating socket\n");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(9999);

    if (connect(sock_desc, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0) {
        printf("Failed to connect to server\n");
    }

    while(1)
    {
        fgets(sbuff, MAX_SIZE , stdin);
        send(sock_desc,sbuff,strlen(sbuff),0);
        
        //Handler for Quit command 'q'
        if(strcmp(sbuff, "q\n") == 0) {
        	break;
        }
        
        //trying to send a string over sockets
        /*if(strcmp(sbuff, "try\n") == 0) {
          printf("Sending String message\n");
        	send(sock_desc, "I am Thomas\n", strlen("I am Thomas\n"), 0);
        }*/

        if(recv(sock_desc,rbuff,MAX_SIZE,0)==0)
            printf("Error\n");
        else
			fputs(rbuff,stdout);
			rbuff[strcspn(rbuff, "\n")] = 0;
			if (strcmp(rbuff, "sending the file") == 0) {
				printf("I am listening\n");
				//FILE * write_fd;
				write_fd = open ("./copied file.txt", O_WRONLY | O_CREAT);
				
				fputs(rbuff,stdout);

				sendfile(sock_desc, write_fd, 0, MAX_SIZE);

				
				close(write_fd);
			}

        sleep(2);
    }
    close(sock_desc);
    return 0;
}
