#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sched.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0


struct parser {
	int nregs;//number of registers
	char *endpoint;
};
typedef struct parser parserT;


char *parseCommand(int *nregs, unsigned short *regs, char *ans);
int parseArgs(parserT *parser, int argc, char *argv[]);




