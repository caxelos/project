//
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <string.h>
#include <parser.h>

#define TRUE 1
#define FALSE 0
#define MSG_SIZE 100


void *Sender();
void *Receiver();


int initialize_client(parserT *parser);
int run_client();
void printATcommands();
int close_client();






