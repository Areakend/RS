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
 	char tmpDir[128];
    //Cas des erreurs
    if(pid < 0){
      return;
    }

    if(pid==0){
		if (strcmp(every_word[0],"pwd") == 0) {
			getcwd(tmpDir, sizeof(tmpDir));
         	printf("%s\n",tmpDir);
        	exit(0);
          
        } else {
          execvp(every_word[0],every_word);
        }
         // The first word is the command, then it's the arguments
        // If failed to execute
        printf("Can't execute \n");
        exit(1);
    }
    else
    {
        WaitProcess(pid);
    }
}

void execCD() {
	char user[128];
	char tmpDir[128];
	char tmpDir2[128];
	char temp[10];
	strcpy(tmpDir,"/home");
	strcpy(temp,"/");
	if (strcmp(every_word[1],"~")==0) {
		strcpy(user, getenv("USER"));
		strcat(tmpDir,temp);
		strcat(tmpDir,user);
		chdir(tmpDir);
	}
	else {
		getcwd(tmpDir, sizeof(tmpDir));
		chdir(every_word[1]);
		getcwd(tmpDir2, sizeof(tmpDir2));
		if (strcmp(tmpDir,tmpDir2)==0) {
			printf("Folder %s not found\n",every_word[1]);
		}
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


void clearTab() {
  int i;
  for (i=0;i<MAX_ARGS;i++) {
    every_word[i] = NULL;
  }
}

void inputParser(char *buffer) {
  char *token;
  token = malloc(MAX_ARGS_SIZE * sizeof(char));
  int spaceOffset = preprocessing(buffer);

  char *buff = buffer + spaceOffset;

  //Avancée dans le buffer
  int i;
  //Avancée dans le mot en cours
  int i2 = 0;
  //Avancée dans l'index de tableaux de mots
  int i3 = 0;

  for(i = 0; i < strlen(buff)+1; i++) {
    switch (buff[i]) {
    case '\0':
    case ' ':
    {
      token[i2] = '\0';
      every_word[i3] = malloc(sizeof(token));
      strcpy(every_word[i3],token);
      free(token);
      token = malloc(MAX_ARGS_SIZE * sizeof(char));
      i2 = 0;
      i3++;
    }
      break;
    case '\n':
      break;
    default:
      {
        token[i2] = buff[i];
        i2++;
      }
    }
  }

}

int preprocessing(char *buffer) {
  int i = 0;
  while(buffer[i] == ' ') {
    i++;
  }
  return i;
}

int main(int argc, char *argv[]) {
	char buffer[MAX_INPUT_SIZE];

  displayUserPath();
	while(1){
	   if(!fgets(buffer,sizeof(buffer)-1,stdin)){
	      printf("\n");
        exit(0);
   	 }
     clearTab();
     inputParser(buffer);
     if (strcmp(every_word[0],"cd") == 0) {
     	execCD();
     	strcpy(currentDir, " ");
     }
     else if (strcmp(every_word[0],"exit") == 0) {
     	exit(0);
     }
     else {
     	execPrgm();
     }
     displayUserPath();
  }
}
