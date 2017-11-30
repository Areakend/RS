#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#ifndef ALL_H
#define ALL_H

// Variable Globale
char buffer[64];
char* every_word[64];
char currentDir[128];

/*#define MAX_ARGS 16
#define MAX_ARGS_SIZE 64

// Variable Globale
char buffer[64];
char every_word[MAX_ARGS][MAX_ARGS_SIZE];
char currentDir[128];*/


// cut_buffer.c
void cut_buffer(char* buffer);

void displayUserPath();


#endif
