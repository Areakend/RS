#include "all.h"


void WaitProcess(pid_t pid)
{

    while(1)
    {
        int status;
        int r = waitpid(pid,&status,0);
 
        if(r < 0)
        {
            if(errno == EINTR)
                continue;
            else           
                break;
        }
        break;
    }
}




void execPrgm(){
    pid_t pid;

    pid = fork();
    if(pid < 0){
        return;
    }
 
    if(pid==0){

        execvp(every_word[0],every_word); // The first word is the command, then it's the arguments
         
        // If failed to execute
        printf("Can't execute \n");
        exit(1);
    }
    else
    {
        WaitProcess(pid);
    }
}
 
int main() {
	char buffer[64];
	while(1){
		printf("Wainting command : ");
		if(!fgets(buffer,sizeof(buffer)-1,stdin)){
	        printf("\n");
    	    exit(0);
   		}
		if(strcmp("exit",buffer)==0){
			exit(0);
		}
		if(strcmp("help",buffer)==0){
			while(1){
				printf("Command list : \n");
				printf("help \nexit\n");
				printf("Tap \"exit\" for quit\n");
				printf("Tap the name of a command for more informations\n");
			scanf("%s", buffer);
				if(strcmp("exit",buffer)==0){
					printf("Exit the shell");
					break;
				}
				if(strcmp("help",buffer)==0){
					printf("Displays all the commands\n");
					break;
				}	
			}
		}
		else{
			cut_buffer();
			execPrgm();
		}
	}
}	
