#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#ifndef ALL_H
#define ALL_H
#define MAX_ARGS 20
#define MAX_INPUT_SIZE 256
#define MAX_PIPES 5
#define MAX_SEMICOLON 5
#define MAX_AND_OR 5
#define ANDORALIAS "ù"

// --------- Globals
char buffer[MAX_INPUT_SIZE];
char* lines[MAX_SEMICOLON];
char* andors[MAX_SEMICOLON][MAX_AND_OR];
char* pipes[MAX_SEMICOLON][MAX_AND_OR][MAX_PIPES];
char* commands[MAX_SEMICOLON][MAX_AND_OR][MAX_PIPES][MAX_ARGS];

int indexOfSemicolon[MAX_SEMICOLON];
int indexOfAndOr[MAX_SEMICOLON][MAX_AND_OR][2]; // 0 = indexOfAndOr ; 1 -> 0 if OR 1 if AND
int indexOfPipe[MAX_SEMICOLON][MAX_AND_OR][MAX_PIPES];

int nbrOfLines; //Define by number of semicolon + 1;
int nbrOfAndOrs[MAX_SEMICOLON]; //Define by number of && or || in line + 1;
int nbrOfPipes[MAX_SEMICOLON][MAX_AND_OR]; //Define by number of pipes in line and in andor + 1;
int nbrOfCommands[MAX_SEMICOLON][MAX_AND_OR][MAX_PIPES];

// --------- Globals

// --------- Prototypes
void clearTab();

void displayUserPath();

int preprocessing(char *buffer);
char* copyBuffer(char *buffer);
char** backgroundArray(char **array);
void affectAndOrAlias(int line);

void splitBySemicolon(char *buffer);
void splitByAndOr(char *buffer, int line);
void splitByPipe(char *buffer, int line, int andor);
void inputParser(char* buffer);

void execCD();
void execPWD();
void execRedirect(char *fichier);

void execWithPriority(const char *command, char *const argv[]);

void execSemicolon(int line); //Précédement execPtVirgule
void execAndOr(int line); //Précédement execPtVirgule
int execPipe(int line, int andor, int inBackground);

int checkSemicolon(char *buffer); //Précédement commandAfterV
int checkAndOr(char *buffer, int line);
int checkPipe(char *buffer, int line, int andor);

// --------- Prototypes


#endif
