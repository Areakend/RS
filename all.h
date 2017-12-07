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

// Variable Globale
char buffer[64];
char* every_word[MAX_ARGS];
int indexOfPipe[32];

void inputParser(char* buffer);
int preprocessing(char *buffer);
void execCD();
void execPWD();
void execRedirect(char *fichier);
void clearTab();

void displayUserPath();


#endif
