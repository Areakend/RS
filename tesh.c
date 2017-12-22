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
  int semicolonIndex = 0;
  char* token;

  token = strtok(buffer, " ");
  while(token != NULL) {
    if (strcmp(token,";") == 0) {
      indexOfSemicolon[semicolonIndex] = i;
      semicolonIndex++;
    }
    token = strtok(NULL, " ");
    i++;
  }
  nbrOfLines = semicolonIndex + 1;
  return semicolonIndex;
}

int checkAndOr(char *buffer, int line) {
  int i = 0;
  int andorIndex = 0;
  char* token;

  token = strtok(buffer, " ");
  while(token != NULL) {
    if (strcmp(token,"||") == 0 || strcmp(token,"&&") == 0) {
      indexOfAndOr[line][andorIndex][0] = i;
      if (strcmp(token,"||") == 0) {
        indexOfAndOr[line][andorIndex][1] = 0;
      } else {
        indexOfAndOr[line][andorIndex][1] = 1;
      }
      andorIndex++;
    }
    token = strtok(NULL, " ");
    i++;
  }
  nbrOfAndOrs[line] = andorIndex + 1;
  return andorIndex;
}

int checkPipe(char *buffer, int line, int andor) {
  int i = 0;
  int pipeIndex = 0;
  char* token;

  token = strtok(buffer, " ");
  while(token != NULL) {
    if (strcmp(token,"|") == 0) {
      indexOfPipe[line][andor][pipeIndex] = i;
      pipeIndex++;
    }
    token = strtok(NULL, " ");
    i++;
  }
  nbrOfPipes[line][andor] = pipeIndex + 1;
  return pipeIndex;
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

void splitByAndOr(char *buffer, int line) {

  char *andor = strtok(buffer, ANDORALIAS);

  int i = 0; int offset = 0;

  while(andor != NULL) {
    offset = preprocessing(copyBuffer(andor));
    andors[line][i] = malloc(sizeof(andor + offset));
    strcpy(andors[line][i], andor + offset);
    andor = strtok(NULL, ANDORALIAS);
    i++;
  }
}

void splitByPipe(char *buffer, int line, int andor) {
  char *pipe = strtok(buffer, "|");
  int i = 0; int offset = 0;

  while(pipe != NULL) {
    offset = preprocessing(copyBuffer(pipe));
    pipes[line][andor][i] = malloc(sizeof(pipe + offset));
    strcpy(pipes[line][andor][i], pipe + offset);
    pipe = strtok(NULL, "|");
    i++;
  }
}

/*

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
}*/


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
  int s; int ao; int p; int a;
  nbrOfLines = 0;
  for (s = 0; s < MAX_SEMICOLON; s++) {
    for (ao = 0; ao < MAX_AND_OR; ao++) {
      for (p = 0; p < MAX_PIPES; p++) {
        for (a = 0; a < MAX_ARGS; a++) {
          commands[s][ao][p][a] = NULL;
        }
        pipes[s][ao][p] = NULL;
        indexOfPipe[s][ao][p] = 0;
      }
      andors[s][ao] = NULL;
      nbrOfPipes[s][ao] = 0;
      indexOfAndOr[s][ao][0] = 0;
      indexOfAndOr[s][ao][1] = 0;
    }
    lines[s] = NULL;
    nbrOfAndOrs[s] = 0;
    indexOfSemicolon[s] = 0;
  }
}

char* copyBuffer(char *buffer) {
  char *copy = malloc(strlen(buffer)+1);
  strcpy(copy, buffer);
  return copy;
}

void affectAndOrAlias(int line) {
  char *buffer = copyBuffer(lines[line]);
  char *token = strtok(buffer, " ");
  char *newLine = malloc(sizeof(lines[line]) + 2 * sizeof(char));

  while(token != NULL) {
    if ((strcmp(token, "||") == 0) || (strcmp(token, "&&") == 0)) {
      strcat(newLine, ANDORALIAS);
    } else {
      strcat(newLine, token);
    }
    strcat(newLine, " ");
    token = strtok(NULL, " ");
  }

  lines[line] = NULL;
  lines[line] = malloc(sizeof(newLine));
  strcpy(lines[line], newLine);
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

void inputParser(char *buffer) {
  const char *delimiter = " ";
  char *token;
  int line = 0; int andor = 0; int pipe = 0; int indexCommand = 0;

  int spaceOffset = preprocessing(buffer);
  char *unspacedBuffer = buffer + spaceOffset;

  splitBySemicolon(copyBuffer(unspacedBuffer));
  checkSemicolon(copyBuffer(unspacedBuffer));

  for (line = 0; line < nbrOfLines; line++) {
    checkAndOr(copyBuffer(lines[line]), line);
    affectAndOrAlias(line);
    splitByAndOr(copyBuffer(lines[line]), line);

    for (andor = 0; andor < nbrOfAndOrs[line]; andor++) {
      splitByPipe(copyBuffer(andors[line][andor]), line, andor);
      checkPipe(copyBuffer(andors[line][andor]), line, andor);

      for (pipe = 0; pipe < nbrOfPipes[line][andor]; pipe++) {
        indexCommand = 0;
        token = strtok(pipes[line][andor][pipe], delimiter);
        while(token != NULL) {
          commands[line][andor][pipe][indexCommand] = malloc(sizeof(token));
          strcpy(commands[line][andor][pipe][indexCommand], token);
          //printf("Line %d - AndOr %d - Pipe %d - Command %d : %s\n", line, andor, pipe, indexCommand, commands[line][andor][pipe][indexCommand]); //Afficher tout !
          token = strtok(NULL, delimiter);
          indexCommand++;
        }
        nbrOfCommands[line][andor][pipe] = indexCommand;
      }
    }
  }

}

/*void execWithPriority(int commandIndex) {
  char *semiColon = commandAfterV(commandIndex);

  if (command2 != NULL) {
    execSemicolon(commandIndex);
  }

  execvp(commands[commandIndex][0], (char * const *)commands[commandIndex])
}*/

int pipeRedirect(char*** nextCommand, int buffin, int buffout) {
  /*pid_t pid = fork ();
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
  return pid;*/
}

void execPipe(int line, int andor) {
/*  pid_t pid;

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

  execvp(commands[line][k][0], (char * const *)commands[line][k]);*/
}

void execAndOr(int line, int andor) {

  if (indexOfAndOr[line][andor][1] == 0) { //Cas ||

  } else { //Cas &&

  }
}

void execSemicolon(int line) {
  pid_t pid;
  int andor = 0;

  pid = fork();
  if(pid < 0){
    printf("Error during Fork in : run()\n");
    return;
  }

  if (pid > 0) {
    WaitProcess(pid);
  } else {
    for (andor = 0; andor < nbrOfAndOrs[line]; andor++) {
      execAndOr(line, andor);
    }
  }
}

int main(int argc, char *argv[]) {

	if (!isatty(0)) {
		while(fgets(buffer,sizeof(buffer)-1,stdin)!=NULL) {
    	clearTab();
    	inputParser(buffer);
      //run();
		}
	} else {
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

        //run();
        displayUserPath();
    }
  }
}
