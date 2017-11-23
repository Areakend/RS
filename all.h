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

// cut_buffer.c
void cut_buffer(char* buffer);


#endif
