#include <clientMW.h>


int main(int argc, char *argv[]) { 

	if (initialize_client(argv)==-1)
		return -1;

	if (run_client()==-1)
		return -1;
	
	if (close_client()==-1)
		return -1;


	
	return 0;
}