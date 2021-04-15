#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>



int client_endpoint;


int parse_command(char *cmd) {
	if (cmd[0] != 'A' || cmd [1] != 'T' || cmd[2] != '+') {
		printf("*** Error: Commands must begin with keywork \"AT+\"\n");
		return -1;
	}
	cmd=cmd+3;





	return 0;
}


void *Sender() {


}

void *Receiver() {


}

void *send_request(void *) {


	
}

int main(int argc, char *argv[]) {
	char cmd[100];
	pthread_t S_tid, R_tid;

	
	client_endpoint = open( argv[1], O_RDWR | O_CREAT, S_IRWXU );
	if (client_endpoint == -1 )  {
		printf("Error opening output file. Exiting\n");
		return -1;
    }

    //Create sender's thread
	if (pthread_create( &S_tid,NULL, (void *)Sender, NULL) != 0) {
		printf("Error creating thread. Exiting\n");
		return -1;
	}

	//Create Receiver's thread
	if (pthread_create( &R_tid,NULL, (void *)Receiver, NULL) != 0) {
		printf("Error creating thread. Exiting\n");
		return -1;
	}


	/*
	 * MAIN FUNCTION
	 */
	printf("Welcome to the app.\n");
	while (1) {
		printf("Please enter command: ");
		scanf("%s", cmd);
		if (parse_command(cmd) == -1)
			continue;
		sendRequest(reqBuffer, &req);
		makeCalc(&req);
		getReply(reqBuffer, &req);
		break;


	}

	return 0;
}