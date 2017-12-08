#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#ifndef ALL_H
#define ALL_H
#define MAX_ARGS 16
#define MAX_ARGS_SIZE 64
#define MAX_INPUT_SIZE 256
#define MAX_PIPES 32

// Variable Globale
char buffer[64];
char* every_word[MAX_ARGS];
int nbrOfWord;
int indexOfPipe[32];
int currentPipe = 0;

void inputParser(char* buffer);
int preprocessing(char *buffer);
void execCD();
void execPWD();
void execRedirect(char *fichier);
void clearTab();
char *commandAfterV();
void execPtVirgule();
int checkPipe(char *const argv[]);
void execPipe(int startIndex, int pipeIndex, int stopIndex);
void runCommand(const char *first, char *const argv[]);


void displayUserPath();


#endif
