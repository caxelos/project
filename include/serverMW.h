#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sched.h>
#include <ctype.h>
#include <parser.h>

#define _UNIX03_SOURCE
#define TRUE 1
#define FALSE 0
#define MAX_SIZE 100

void *Sender();
void *Receiver();

int initialize_server(parserT *parser);
int run_server(parserT *parser);
char *parseCommand(int *nregs, char *ans);
int close_server();






