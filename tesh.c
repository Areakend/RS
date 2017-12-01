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


/*void clearBuffer() {
  int i;
  for(i=0;i<MAX_ARGS;i++) {
    free(every_word[i]);
  }
}*/

void execPrgm(){
    //clearBuffer();
    pid_t pid;
    pid = fork();
    //char command4[64];
    //printf("%s \n", command);

    //Cas des erreurs
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

void displayUserPath() {
 char *parsing = NULL;
 char tmpDir[128];
 char tmpDir2[128];
 char hostname[64];
 int parseRange = 2, i, b = 0;
 getcwd(tmpDir, sizeof(tmpDir));
 parsing = strtok(getcwd(tmpDir2, sizeof(tmpDir2)),"/");

 while (strcmp(parsing,getenv("USER"))) {
   parsing = strtok(NULL,"/");
   parseRange++;
 }

 for (i = 0; i < strlen(tmpDir);i++) {
   if (tmpDir[i] == '/' && b < parseRange) {
     b++;
   }
   if (b == parseRange) {
     memcpy(currentDir, &tmpDir[i], &tmpDir[strlen(tmpDir)]- &tmpDir[i]);
     b++;
   }
 }

 gethostname(hostname,sizeof(hostname));
 printf("%s@%s:~%s$ ",getenv("USER"),hostname,currentDir);

}

int main(int argc, char *argv[]) {
	char buffer[MAX_INPUT_SIZE];

  displayUserPath();
	while(1){
	   if(!fgets(buffer,sizeof(buffer)-1,stdin)){
	      printf("\n");
        exit(0);
   	 }
     inputParser(buffer);
		 //cut_buffer(buffer);
     execPrgm();
     displayUserPath();
  }
}
