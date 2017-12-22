#include "all.h"

void WaitProcess(pid_t pid) {
  while(1)
  {
      int status;
      int r = waitpid(pid,&status,0);
      if(r < 0) {
        if(errno == EINTR)
          continue;
        else
          break;
      }
      break;
  }
}

int checkSemicolon(char *buffer) {
  int i = 0;
  int nbrOfSemicolon = 0;
  char* token;

  token = strtok(buffer, " ");
  while(token != NULL) {
    if (strcmp(token,";") == 0) {
      indexOfSemicolon[nbrOfSemicolon] = i;
      nbrOfSemicolon++;
    }
    token = strtok(NULL, " ");
    i++;
  }
  nbrOfLines = nbrOfSemicolon + 1;
  return nbrOfSemicolon;
}

void splitBySemicolon(char *buffer) {
  char *line = strtok(buffer, ";");
  int i = 0; int offset = 0;

  while(line != NULL) {
    offset = preprocessing(copyBuffer(line));
    lines[i] = malloc(sizeof(line + offset));
    strcpy(lines[i], line + offset);
    line = strtok(NULL, ";");
    i++;
  }
}

int checkPipe(char *buffer, int line) {
  int i = 0;
  int nbrOfPipe = 0;
  char* token;

  token = strtok(buffer, " ");
  while(token != NULL) {
    if (strcmp(token,"|") == 0) {
      indexOfPipe[line][nbrOfPipe] = i;
      nbrOfPipe++;
    }
    token = strtok(NULL, " ");
    i++;
  }
  nbrOfCommands[line] = nbrOfPipe + 1;
  return nbrOfPipe;
}

/*
void execSemicolon() {
  char *cmd2 = strok(every_word, ";")
  while (cmd!=NULL) {
    execprgm(cmd2[0], cmd2);
    cmd2= strtok(NULL, ";")
}
}*/

/*char *commandAfterA() {
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
}*/

int pipeRedirect(char*** nextCommand, int buffin, int buffout) {
  pid_t pid = fork ();
  if (pid == 0) {
    if (buffout != 1) {
      dup2 (buffout, 1);
      close (buffout);
    }
    if (buffin != 0) {
      dup2 (buffin, 0);
      close (buffin);
    }
    return execvp(nextCommand[0], (char * const *) nextCommand);
  }
  return pid;
}

void execPipe(int line) {
  pid_t pid;

  int pipefd[2], k, buffin;
  buffin = 0;

  for(k=0; k < nbrOfCommands[line] - 1; k++) {
    pipe(pipefd);
    pipeRedirect(commands[line] + k, buffin, pipefd[1]);
    close(pipefd[1]);
    buffin = pipefd[0];
  }

  if (buffin != 0) {
    dup2(buffin, 0);
  }

  execvp(commands[line][k][0], (char * const *)commands[line][k]);
}

/*void execRedirect(char *fichier) {
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
}*/

void displayUserPath() {
  int len = strlen(getenv("HOME"));
  char tmpDir[128];
  char hostname[64];
  gethostname(hostname,sizeof(hostname));
  getcwd(tmpDir, sizeof(tmpDir));
  printf("%s@%s:~%s$ ",getenv("USER"),hostname,tmpDir+len);
}

void clearTab() {
  int i; int j; int k;
  for (k = 0; k < MAX_SEMICOLON; k++) {
    for (i = 0; i < MAX_PIPES; i++) {
      for (j = 0; j < MAX_ARGS; j++) {
        commands[k][i][j] = NULL;
      }
      indexOfPipe[k][i] = 0;
    }
  }
}

char* copyBuffer(char *buffer) {
  char *copy = malloc(strlen(buffer)+1);
  strcpy(copy, buffer);
  return copy;
}

void inputParser(char *buffer) {
  const char *delimiter = " ";
  char *token;
  int line = 0;
  int nbrOfPipe;

  int index;
  int index2;
  int nextPipe;
  int pos;
  int indexPipe;


  int spaceOffset = preprocessing(buffer);
  char *unspacedBuffer = buffer + spaceOffset;
  int limit = strlen(unspacedBuffer);

  splitBySemicolon(copyBuffer(unspacedBuffer));
  checkSemicolon(copyBuffer(unspacedBuffer));

  for (line = 0; line < nbrOfLines; line++) {
    nbrOfPipe = checkPipe(copyBuffer(lines[line]), line);

    index = 0;
    index2 = 0;
    nextPipe = 0;
    pos = 0;

    if (nbrOfPipe == 0) {
      indexPipe = limit;
    } else {
      indexPipe = (uintptr_t) indexOfPipe[line][nextPipe];
    }

    token = strtok(lines[line], delimiter);
    while(token != NULL) {
      if (index < indexPipe) {
        commands[line][pos][index2] = malloc(sizeof(token));
        strcpy(commands[line][pos][index2],token);
        index2++;
      } else {
        nextPipe++;
        if (nextPipe == nbrOfPipe) {
          indexPipe = limit;
        } else {
          indexPipe = (uintptr_t) indexOfPipe[line][nextPipe];
        }
        pos++;
        index2 = 0;
      }
      token = strtok(NULL, delimiter);
      index++;
    }

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

/*void execWithPriority(int commandIndex) {
  char *semiColon = commandAfterV(commandIndex);

  if (command2 != NULL) {
    execSemicolon(commandIndex);
  }

  execvp(commands[commandIndex][0], (char * const *)commands[commandIndex])
}*/

void run(int line) {
  pid_t pid;

  pid = fork();
  if(pid < 0){
    printf("Error during Fork in : run()\n");
    return;
  }

  if (pid > 0) {
    WaitProcess(pid);
  } else {
    execPipe(line);
  }
}


int main(int argc, char *argv[]) {

//	if (argc==1) {
//	if (isatty(fileno(stdin))) {
	if (!isatty(0)) {
//		FILE *file = fopen(argv[1], "r");
		while(fgets(buffer,sizeof(buffer)-1,stdin)!=NULL) {

		
		clearTab();
		inputParser(buffer);
		 int line;
		 for(line = 0; line < nbrOfLines; line++) {
		   run(line);
		 }
		 }
	}
	
	else {
		displayUserPath();
		while(1){
		   if(!fgets(buffer,sizeof(buffer)-1,stdin)){
		   if (argc>1) {
			  printf("\n");
		   }
		    exit(0);
	   	 }
		 clearTab();
		 inputParser(buffer);

		 int line;
		 for(line = 0; line < nbrOfLines; line++) {
		   run(line);
		 }
	  displayUserPath();
     }
  }

}
