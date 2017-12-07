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

char *file() {
	int i=0;
	char *inFile=NULL;
	while (every_word[i]!=NULL) {
		if (strcmp(every_word[i],">")==0) {
			inFile = every_word[i+1];
			while (every_word[i+2]!=NULL) {
				every_word[i] = every_word[i+2];
				i++;
			}
			every_word[i]=NULL;
		}
		i++;
	}
	return(inFile);
}

int checkPipe() {
  int i = 0;
  int nbrOfPipe = 0;
  while (every_word[i]!=NULL) {
	   if (strcmp(every_word[i],"|")==0) {
       indexOfPipe[nbrOfPipe] = i;
       nbrOfPipe++;
     }
     i++;
   }
   return nbrOfPipe;
}

void execPrgm(){
    char *fichier = file();
    pid_t pid;
    pid = fork();

    //Cas des erreurs
    if(pid < 0){
      return;
    }

    if(pid==0){

    	if (fichier != NULL) {
  	    int redirect = (int)freopen(fichier, "w", stdout);
    		if (redirect == -1) {
      		printf("Error file\n");
      		exit(0);
  	    }
   		}
      execvp(every_word[0],every_word);
      printf("Can't execute \n");
      exit(0);
    }
    else
    {
        WaitProcess(pid);
    }
}

void execRedirect() {
    char *fichier = file();
	  pid_t pid;
    pid = fork();
    //Cas des erreurs
    if(pid < 0){
      return;
    }
    int redirect = (int)freopen(fichier, "w", stdout);
    if (redirect == -1) {
    	printf("Error file\n");
    	exit(0);
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

void execPWD() {
  char tmpDir[128];
  getcwd(tmpDir, sizeof(tmpDir));
  printf("%s\n",tmpDir);
}

void displayUserPath() {
  int len = strlen(getenv("HOME"));
  char tmpDir[128];
  char hostname[64];
  gethostname(hostname,sizeof(hostname));
  getcwd(tmpDir, sizeof(tmpDir));
  printf("%s@%s:~%s$ ",getenv("USER"),hostname,tmpDir+len);
}

void clearTab() {
  int i;
  for (i=0;i<MAX_ARGS;i++) {
    every_word[i] = NULL;
  }
}

void inputParser(char *buffer) {
  const char *delimiter = " ";
  char *token;
  int spaceOffset = preprocessing(buffer);
  int i;
  char *buff = buffer + spaceOffset;

  token = strtok(buff, delimiter);

  while(token != NULL) {
    every_word[i] = malloc(sizeof(token));
    strcpy(every_word[i],token);
    token = strtok(NULL, delimiter);
    i++;
  }
}

int preprocessing(char *buffer) {
  int i = 0;
  while(buffer[i] == ' ') {
    i++;
  }
  if (buffer[strlen(buffer)-1] == '\n') {
    buffer[strlen(buffer)-1] = '\0';
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

     if (strcmp(every_word[0],"") != 0) { //Si Non Entrée sans rien
       if (strcmp(every_word[0],"cd") == 0) {
         execCD();
       }
       else if (strcmp(every_word[0],"pwd") == 0) {
         execPWD();
       }
       else if (strcmp(every_word[0],"exit") == 0) {
         exit(0);
       }
       else {
         execPrgm();
       }
     }
     displayUserPath();
  }
}
