
/*//
 *
 */
#include <clientMW.h>

int main(int argc, char *argv[]) {
	parserT parser;

	if (parseArgs(&parser, argc, argv) == -1)
		return -1;

	if (initialize_client(&parser)==-1)
		return -1;

	if (run_client()==-1)
		return -1;
	
	if (close_client()==-1)
		return -1;


	
	return 0;
}