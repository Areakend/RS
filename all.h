#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#ifndef ALL_H
#define ALL_H
#define MAX_ARGS 16
#define MAX_ARGS_SIZE 64
#define MAX_INPUT_SIZE 256
#define MAX_PIPES 32
#define STDIN_FILENO 0
#define STDOUT_FILENO 1

// Variable Globale
char buffer[MAX_INPUT_SIZE];
char* every_word[64];
char* commands[MAX_PIPES][MAX_ARGS];
int indexOfPipe[MAX_PIPES];
int static child = 0;

void inputParser(char* buffer);
int preprocessing(char *buffer);
void execCD();
void execPWD();
void execRedirect(char *fichier);
void clearTab();
char *commandAfterV();
void execPtVirgule();
int checkPipe(char *buffer);
void execPipe(int pos, int in_fd);
void runCommand(const char *first, char *const argv[]);


void displayUserPath();


#endif
