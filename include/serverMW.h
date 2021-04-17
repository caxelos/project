#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sched.h>
#include <ctype.h>

#define _UNIX03_SOURCE
#define TRUE 1
#define FALSE 0
#define MAX_SIZE 20


struct parser {
	int nregs;//number of registers
	char *endpoint;
};
typedef struct parser parserT;


int parseArgs( parserT *parser, int argc, char *argv[]);
int initialize_server(parserT *parser);
void *Receiver();
int run_server(parserT *parser);
char *parseCommand(int *nregs, unsigned short *regs, char *ans);
int close_server();






