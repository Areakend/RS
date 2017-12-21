#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#ifndef ALL_H
#define ALL_H
#define MAX_ARGS 16
#define MAX_INPUT_SIZE 256
#define MAX_PIPES 32
#define MAX_SEMICOLON 32

// --------- Globals
char buffer[MAX_INPUT_SIZE];
char* lines[MAX_SEMICOLON];
char* commands[MAX_SEMICOLON][MAX_PIPES][MAX_ARGS];

int indexOfSemicolon[MAX_SEMICOLON];
int indexOfPipe[MAX_SEMICOLON][MAX_PIPES];
int nbrOfLines; //Define by number of semicolon + 1;
int nbrOfCommands[MAX_SEMICOLON]; //Define by number of pipes in line + 1;
// --------- Globals

// --------- Prototypes
void splitBySemicolon(char *buffer);
void inputParser(char* buffer);
int preprocessing(char *buffer);
char* copyBuffer(char *buffer);
void displayUserPath();
void clearTab();

void execCD();
void execPWD();
void execRedirect(char *fichier);
void execSemicolon(int commandIndex); //Précédement execPtVirgule
void execPipe(int line);
void execWithPriority(int commandIndex);

int checkSemicolon(char *buffer); //Précédement commandAfterV
int checkPipe(char *buffer, int line);

void run(int line);
// --------- Prototypes


#endif
