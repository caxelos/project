#include <parser.h>


/* function parseArgs()
 *
 * - extracts the command line arguments given by the user 
 */
int parseArgs( parserT *parser, int argc, char *argv[]) {
	int i;
	parser->nregs=5;
	parser->endpoint=NULL;

	if (argc == 1) {
		printf("Insufficient arguments. Supported arguments are:\n\t\"-endpoint\": provide device (e.g. /dev/pts/0)\n\t\"-nreg\": number of register (for server only). Default is 5\n\n");
		return -1;
	}


	/*
	 * overwrite if needed
	 */
	for (i=1; i < argc; i++) {
		if (argv[i][0] == '-') {
			if (strcmp(argv[i],"-nregs") == 0) {
				/*
				 * only for the Server
				 */
				i++;
				parser->nregs=atoi(argv[i]);
			}
			else if (strcmp(argv[i],"-endpoint") == 0) {
				/*
				 * device path, eg. /dev/pts/2
				 */

				i++;
				parser->endpoint=argv[i];
			}
		}
		else {
			printf("Error: Wrong argument or should start with \"-\" character.Type help to see the supported commands. \n");
			return -1;
		}
	}

	if (strcmp(parser->endpoint,"\0")==0) {
		printf("Error. \"-endpoint\" argument missing (e.g. -endpoint /dev/pts1).\n");
		return -1;
	}

	return 0;
}