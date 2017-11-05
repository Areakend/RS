#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#ifndef ALL_H
#define ALL_H

// variable globale
char buffer[64];
char* every_word[64];


// main.c
int main();

// cut_buffer.c
void cut_buffer();


#endif
