#include <stdio.h>
#include <string.h>
#include <stdlib.h>



int main(){
    
    int ret;
    int strlength;
    char simple_command[100];
    char command[2];
    char filename[20];
    int i,j,cnt;
    
    j=0; cnt=0;
    
    while(1){
        gets(simple_command);
        strlength = strlen(simple_command);
        ret = strcmp(simple_command, "q");
        if(ret == 0){
            break;
        }
        if(strlength == 1){
            //print the list here in form OK+ name.type
            printf("%s\n","print list of file name here");
        }
        else{
            
            for(i=0;i<=strlength;i++){
                if(i == 0){
                    command[i] = simple_command[i];
                }
                if(cnt > 0){
                    filename[j] = simple_command[i];
                    j++;
                }
                if(simple_command[i]==' '){
                    cnt++;  //for next word
                    j=0;    //for next word, init index to 0
                }
            }
            
            if(strcmp(command, "u") == 0){
                printf("do uploading %s here\n",filename);
            }
            else if(strcmp(command, "r") == 0){
                printf("do removing %s here\n",filename);
            }
            else if(strcmp(command, "d") == 0){
                printf("do downloading %s here\n",filename);
            }
            else{
                printf("%s\n","no such command");
            }
            
        }
        
    }
    
    return 0;
}

