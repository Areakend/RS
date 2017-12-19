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

char *commandAfterV() {
	int i=0;
	char *inFile=NULL;
	while (every_word[i]!=NULL) {
		if (strcmp(every_word[i],";")==0) {
			inFile = every_word[i+1];
			while (every_word[i+2]!=NULL) {
				every_word[i] = every_word[i+2];
				i++;
			}
			every_word[i]=NULL;
			return(inFile);
		}
		i++;
	}
	return(inFile);
}

char *commandAfterA() {
	int i=0;
	char *inFile=NULL;
	while (every_word[i]!=NULL) {
		if (strcmp(every_word[i],"&")==0) {
			if (strcmp(every_word[i],"&")==0) {
				inFile = every_word[i+2];
				while (every_word[i+2]!=NULL) {
					every_word[i] = every_word[i+2];
					i++;
				}
				every_word[i]=NULL;
				return(inFile);
			}
		}
		i++;
	}
	return(inFile);
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

int checkPipe(char *buffer) {
  int i = 0;
  int nbrOfPipe = 0;
  char* token;

  token = strtok(buffer, " ");
  while(token != NULL) {
    if (strcmp(token,"|") == 0) {
      indexOfPipe[nbrOfPipe] = i;
      nbrOfPipe++;
    }
    token = strtok(NULL, " ");
    i++;
  }
   return nbrOfPipe;
}

void pipe_redirect(int oldfd, int newfd) {
  if (oldfd != newfd) {
    if (dup2(oldfd,newfd) != -1) {
      close(oldfd);
    } else {
      printf("Error in duping\n");
    }
  }
}

void execPipe(int pos, int in_fd) {
  if (commands[pos + 1] == NULL) {
    pipe_redirect(in_fd, STDIN_FILENO);
    // A MODIFIER POUR COMMAND RAPH
    execvp(commands[pos][0], commands[pos]);
    printf("Can't execute\n");
    exit(0);
  } else {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
      printf("Error in piping\n");
    }

    switch(fork()) {
      case -1:
        printf("Error in forking\n");
      case 0:
        child = 1;
        close(pipefd[0]);
        pipe_redirect(in_fd, STDIN_FILENO);
        pipe_redirect(pipefd[1], STDOUT_FILENO);
        //A MODIFIER POUR COMMAND RAPH
        execvp(commands[pos][0], commands[pos]);
        printf("Can't execute\n");
        exit(0);
      default:
        close(pipefd[1]);
        close(in_fd);
        execPipe(pos + 1, pipefd[0]);
    }
  }
}

void execRedirect(char *fichier) {
//    char *fichier = file();
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

void execPtVirgule() {
	//printf("%s\n",*every_word);
	//char *cmd1 = strdup(every_word);
	//printf("%s\n",every_word);
	char *cmd2 = strtok(every_word,";");
	while (cmd2!=NULL) {
		execPrgm(cmd2[0],cmd2);
		cmd2=strtok(NULL,";");
	}
}

void execAnd() {
	//printf("%s\n",*every_word);
	//char *cmd1 = strdup(every_word);
	//printf("%s\n",every_word);
	char *cmd2 = strtok(every_word,"&&");
	while (cmd2!=NULL) {
		execPrgm(cmd2[0],cmd2);
		cmd2=strtok(NULL,"&&");
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
    every_word[i] = (char*) NULL;
  }
  for (i=0;i<MAX_PIPES;i++) {
    indexOfPipe[i] = (int) NULL;
  }
  //nbrOfWord = 0;
  //currentPipe = 0;
}

void clearTab2() {
  int i; int j;
  for (i = 0; i < MAX_PIPES; i++) {
    for (j = 0; j < MAX_ARGS; j++) {
      commands[i][j] = NULL;
    }
  }
}

void inputParser(char *buffer) {
  const char *delimiter = " ";
  char *token;
  int spaceOffset = preprocessing(buffer);
  int i;
  char *buff = buffer + spaceOffset;

  token = strtok(buff, delimiter);
  //nbrOfWord = 0;
  while(token != NULL) {
    every_word[i] = malloc(sizeof(token));
    strcpy(every_word[i],token);
    token = strtok(NULL, delimiter);
    i++;
    //nbrOfWord++;
  }
}

void inputParser2(char *buffer) {
  const char *delimiter = " ";
  char *token;
  int spaceOffset = preprocessing(buffer);
  char *buff = buffer + spaceOffset;
  int limit = strlen(buff);
  char *buff2 = malloc(strlen(buff)+1);
  strcpy(buff2, buff);
  int nbrOfPipe = checkPipe(buff2);

  int index = 0;
  int index2 = 0;
  int nextPipe = 0;
  int pos = 0;

  int indexPipe;
  if (nbrOfPipe == 0) {
    indexPipe = limit;
  } else {
    indexPipe = indexOfPipe[nextPipe];
  }

  token = strtok(buff, delimiter);
  while(token != NULL) {
    if (index < indexPipe) {
      commands[pos][index2] = malloc(sizeof(token));
      strcpy(commands[pos][index2],token);
      index2++;
    } else {
      nextPipe++;
      if (nextPipe == nbrOfPipe) {
        indexPipe = limit;
      } else {
        indexPipe = indexOfPipe[nextPipe];
      }
      pos++;
      index2 = 0;
    }
    token = strtok(NULL, delimiter);
    index++;
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

void execPrgm(){
    char *fichier = file();
    char *command2 = commandAfterV();
    char *command3 = commandAfterA();
    pid_t pid;
    pid = fork();

    //Cas des erreurs
    if(pid < 0){
      return;
    }

    if(pid==0){

    	if (fichier != NULL) {
  	    	execRedirect(fichier);
  	    }

  	    if (command2 != NULL) {
  	    	execPtVirgule(every_word);
  	    }

  	    if (command3 != NULL) {
  	    	execAnd	(every_word);
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

void runCommand(const char *first, char *const argv[]) {
  int nbrOfPipe = checkPipe(argv);
  char *fichier = file();
  char *command2 = commandAfterV();
  pid_t pid;

  pid = fork();
  if(pid < 0){
    printf("Error during Fork\n");
    return;
  }

  if (pid > 0) {
    WaitProcess(pid);
  } else {
    /*if (currentPipe < nbrOfPipe) {
      if (currentPipe == 0) {
        printf("CAS PREMIER PIPE\n");
        //execPipe(0, indexOfPipe[currentPipe], nbrOfWord-1);
      } else if ((currentPipe+1) == nbrOfPipe) {
        printf("CAS DERNIER PIPE\n");
      } else {
        printf("CAS PIPE\n");
        //execPipe(indexOfPipe[currentPipe-1]+1,currentPipe, nbrOfWord-1);
      }
    } else if (fichier != NULL) {
      execRedirect(fichier);
    } else if (command2 != NULL) {
      execPtVirgule(every_word);
    } else {
      execvp(first, argv);
      printf("RunCommand : Can't execute\n");
      exit(0);
    }*/

  }
}

int main(int argc, char *argv[]) {
	displayUserPath();
	while(1){
	   if(!fgets(buffer,sizeof(buffer)-1,stdin)){
	      printf("\n");
        exit(0);
   	 }
     clearTab2();
     //inputParser(buffer);
     inputParser2(buffer);
     execPipe(0, STDIN_FILENO);

     /*if (strcmp(every_word[0],"") != 0) { //Si Non Entrée sans rien
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
         runCommand(every_word[0],every_word);
       }
     }*/
     displayUserPath();
  }
}
