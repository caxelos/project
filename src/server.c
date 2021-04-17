#include <serverMW.h>

int main(int argc, char *argv[]) {
	parserT parser;

	if (parseArgs(&parser, argc, argv) == -1)
		return -1;


	if (initialize_server(&parser)==-1)
		return -1;


	if (run_server(&parser)==-1)
		return -1;
	

	if (close_server()==-1)
		return -1;
	

	return 0;
}