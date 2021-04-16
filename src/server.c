#include <serverMW.h>

#define MAX_SIZE 20
#define _UNIX03_SOURCE



int main(int argc, char *argv[]) {
	int  i,nbytes;
	char ans[50];
	parserT parser;

	if (parseArgs(&parser, argc, argv) == -1)
		return -1;


	if (initialize_server(&parser)==-1)
		return -1;

	
	unsigned short *regs = (unsigned short *)malloc( parser.nregs*sizeof(unsigned short) );
	for (i=0; i < parser.nregs; i++) {
		regs[i]= rand()%10;
	}

	/*
	 * MAIN FUNCTION
	 */
	while (1) {
		
		if (SIGNAL_READ_FROM_ENDPOINT==FALSE) {
			parseCommand( &(parser.nregs), regs, ans);
			for (i = 0; i < strlen(ans); i++) {
				nbytes = write(server_endpoint,ans+i,sizeof(char));
				if (nbytes == -1)
					printf("problem at write\n");
			}
			nbytes = write(server_endpoint,"\n",sizeof(char));
			SIGNAL_READ_FROM_ENDPOINT=TRUE;
			
		}
		else {
			sched_yield();
		}
	}

	if (close(server_endpoint) < 0) {
		printf("error closing server endpoint. Exiting\n");
		return -1;
	}

	free( regs );
	return 0;
}