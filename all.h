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
char currentDir[128];


/*
// Variable Globale
char buffer[64];
char every_word[MAX_ARGS][MAX_ARGS_SIZE];
char currentDir[128];*/


// cut_buffer.c
void inputParser(char* buffer);
int preprocessing(char *buffer);
//void cut_buffer(char* buffer);

void displayUserPath();


#endif
