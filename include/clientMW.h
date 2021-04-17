#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define MSG_SIZE 100


void *Sender();
void *Receiver();


int initialize_client(char *argv[]);
int run_client();
void printATcommands();
int close_client();






