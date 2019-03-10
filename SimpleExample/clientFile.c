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

int cfileexists(const char * filename){
    /* try to open file to read */
    FILE *file;
		printf("%s\n", filename);
    if (file = fopen(filename, "r")){
		printf("file is here\n");
        fclose(file);
        return 1;
    }
    return 0;
}

int main()
{
    int sock_desc;
    int read_fd;
		int write_fd;
		struct stat stat_buf;
    struct sockaddr_in serv_addr;
    char sbuff[MAX_SIZE],rbuff[MAX_SIZE], filebuff[MAX_SIZE], buffer[1024];
		//off_t offset = 0;

    if((sock_desc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        printf("Failed creating socket\n");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(9989);

    if (connect(sock_desc, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0) {
        printf("Failed to connect to server\n");
    }

    while(1)
    {
        fgets(sbuff, MAX_SIZE , stdin);
        //send(sock_desc,sbuff,strlen(sbuff),0);
        
        //Handler for Quit command 'q'
        if(strcmp(sbuff, "q\n") == 0) {
        	buffer[0] = 0x08;
          send(sock_desc, buffer, 1, 0);
          if(recv(sock_desc,rbuff,MAX_SIZE,0)==0) printf("Error\n"); 
          else {
          	if(rbuff[0] == 0x09) {
          		//close Socket
          		break;
          	} else printf("close failed");
        	}
        }
        
        //Handler for List command 'l'
        if(strcmp(sbuff, "l\n") == 0) {
        	buffer[0] = 0x00;
          send(sock_desc, buffer, 1, 0);
          if(recv(sock_desc,rbuff,MAX_SIZE,0)==0) printf("Error\n");
          else {
						//get full list of filenames
						//print them with OK
        	}
        }
        
        char * cmdsplit = strtok(sbuff, " ");		//NEEDS major testing

        //Handler for Upload command 'u'
        printf("%s\n", cmdsplit);
        if(strcmp(cmdsplit, "u") == 0) {
        	printf("split right\n");
        	buffer[0] = 0x02;
          send(sock_desc, buffer, 1, 0);
          cmdsplit = strtok(NULL, " \n");
          char tmp[MAX_SIZE];
          cmdsplit = strcat(strcpy(tmp,"./"), cmdsplit);		//must be path from current directory
          printf("%s\n", cmdsplit);
          if(recv(sock_desc,rbuff,MAX_SIZE,0)==0) printf("Error\n");
          else {
          	printf("%s\n", rbuff);
        		if(strcmp(rbuff, "Ready") == 0 && cfileexists(cmdsplit)) {
        			printf("File is ready and loaded\n");
        			read_fd = open (cmdsplit, O_RDONLY);
        			fstat (read_fd, &stat_buf);
        			sendfile(sock_desc, read_fd, 0, stat_buf.st_size);
        			//close (read_fd);	
				//close (sock_desc);
			  
        		  if(recv(sock_desc,rbuff,MAX_SIZE-1,0)==0) printf("Error\n");
        		  else {
				if(rbuff[0] != 0x03){
					printf("Updating file is failed.\n");
					}
				else{
					printf("Updating file is successful.\n");
					}
          			}
        		}
		}
        }
	//Handler for remove command 'r'
	if(strcmp(cmdsplit, "r") == 0){
		buffer[0] = 0x04;
		send(sock_desc, buffer, 1, 0);
          	cmdsplit = strtok(NULL, " \n");
		send(sock_desc, cmdsplit, strlen(cmdsplit), 0);
		if(recv(sock_desc,rbuff,MAX_SIZE-1,0)==0) printf("Error\n");
		else{
			if(recv(sock_desc,rbuff,MAX_SIZE-1,0)==0) printf("Error\n");
			else{
				if(rbuff[0] == 0x05){
					printf("Deleted successfully. \n");
				}
				else{
					printf("Deleted failed. File not exist. \n");
				}
			}
		}
	}
	//Handler for download command 'd'
        if(strcmp(cmdsplit, "d") == 0){
	buffer[0] = 0x06;
	send(sock_desc, buffer, 1, 0);
	cmdsplit = strtok(NULL, " \n");
	send(sock_desc, cmdsplit, strlen(cmdsplit), 0);
	if(recv(sock_desc,rbuff,MAX_SIZE-1,0)==0) printf("Error\n");
	else{
		if(recv(sock_desc,rbuff,MAX_SIZE-1,0)==0) printf("Error\n");
		else{
			write_fd = open ("./dfold/file.txt", O_WRONLY | O_CREAT);
			write(write_fd, rbuff, 2000);
			if(recv(sock_desc,rbuff,MAX_SIZE-1,0)==0) printf("Error\n");
			else{
				printf("%s\n",rbuff);
				if(rbuff[0] == 0x07){
					printf("download successfully. \n");
				}
				else{
					printf("Download failed. File not exist. \n");
				}
			}
		}
	}
	}
	// Dead lock here.
        //trying to send a string over sockets
        /*if(strcmp(sbuff, "try\n") == 0) {
          printf("Sending String message\n");
        	send(sock_desc, "I am Thomas\n", strlen("I am Thomas\n"), 0);
        }*/

        //if(recv(sock_desc,rbuff,MAX_SIZE,0)==0)
          //  printf("Error\n");
        //else
	//		fputs(rbuff,stdout);
	//		rbuff[strcspn(rbuff, "\n")] = 0;
	//		if (strcmp(rbuff, "sending the file") == 0) {
	//			printf("I am listening\n");
				//FILE * write_fd;
	//			write_fd = open ("./copied file.txt", O_WRONLY | O_CREAT);
				
	//			fputs(rbuff,stdout);

	//			sendfile(sock_desc, write_fd, 0, MAX_SIZE);

				
	//			close(write_fd);
	//		}

        sleep(2);
    }
    close(sock_desc);
    return 0;
}
