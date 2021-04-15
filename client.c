#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>


int client_endpoint;


int parse_command(char *cmd) {
	if (cmd[0] != 'A' || cmd [1] != 'T' || cmd[2] != '+') {
		printf("*** Error: Commands must begin with keywork \"AT+\"\n");
		return -1;
	}




	return 0;
}


void *Sender() {
	return NULL;

}

void *Receiver() {
	return NULL;

}

void send_request(char *cmd) {

	//SEND=READY;



}

int main(int argc, char *argv[]) {
	char cmd[100];
	//pthread_t S_tid, R_tid;

	
	client_endpoint = open( argv[1], O_RDWR | O_CREAT, S_IRWXU );
	if (client_endpoint == -1 )  {
		printf("Error opening output file. Exiting\n");
		return -1;
    }

 //    //Create sender's thread
	// if (pthread_create( &S_tid,NULL, (void *)Sender, NULL) != 0) {
	// 	printf("Error creating thread. Exiting\n");
	// 	return -1;
	// }

	// //Create Receiver's thread
	// if (pthread_create( &R_tid,NULL, (void *)Receiver, NULL) != 0) {
	// 	printf("Error creating thread. Exiting\n");
	// 	return -1;
	// }


	/*
	 * MAIN FUNCTION
	 */

    int i,nbytes;
	printf("Welcome to the app.\n");
	while (1) {
		printf("Please enter command: ");
		scanf("%s", cmd);
		if (parse_command(cmd) == -1)
			continue;

	

	//code 1	
		for (i = 0; i < strlen(cmd); i++) {
			nbytes = write(client_endpoint,cmd+i,sizeof(char));
			if (nbytes == -1)
				printf("problem at write");
		}
		nbytes = write(client_endpoint,"\n",sizeof(char));
	//code 2
	// char packet[200];
	// sprintf(packet, "echo \"%s\" >  %s",cmd, argv[1]);
	// printf("executing: %s\n", packet);		
	// system(packet);//sendRequest(cmd);


	// 		//sched_yield();
    

	}


		
		//getReply(reqBuffer, &req);
		


	
	return 0;
}