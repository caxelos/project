#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//

struct parser {
	int nregs;//number of registers
	char *endpoint;
};
typedef struct parser parserT;


int parseArgs( parserT *parser, int argc, char *argv[]);