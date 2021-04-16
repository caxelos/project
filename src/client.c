#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>



char ans[100]="\0";
int SIGNAL;


void *Sender() {
	return NULL;

}

volatile int client_endpoint;
void *Receiver(){

/* Receiver thread
	
	while True:
	  - wait until /dev/ttyS0 has data
	  - unpack data:[reqid,data]

	  MUTEX: 
	  	- save them: [reqid, data, status=WAIT]

*/
	
	int i,nbytes;
	
	while (1) {
		i = 0;
		while (1) {
			nbytes = read(client_endpoint, ans+i, sizeof(char) );
			//if (nbytes==0) {
			//	continue;
			//}
			if (nbytes == -1)  {
				printf("Read error at writer() function.Exiting\n");
				return NULL;
    		}

    		if (ans[i]=='\n') {
    			ans[i]='\0';
    			SIGNAL=1;
    			break;
    		}
    	
			i++; 	
    	}


	}


	return NULL;
}

void send_request(char *cmd) {

	//SEND=READY;



}

int main(int argc, char *argv[]) {
	char cmd[100];
	pthread_t R_tid;

	
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

	//Create Receiver's thread
	if (pthread_create( &R_tid,NULL, (void *)Receiver, NULL) != 0) {
		printf("Error creating thread. Exiting\n");
		return -1;
	}


	/*
	 * MAIN FUNCTION
	 */

    int i,nbytes;
	printf("Welcome to the app.\n");
	while (1) {
		printf("\n<--- Please enter command: ");
		scanf("%s", cmd);
		// if (check_validity(cmd) == -1)
		// 	continue;

	

		//code 1	
		for (i = 0; i < strlen(cmd); i++) {
			nbytes = write(client_endpoint,cmd+i,sizeof(char));
			if (nbytes == -1)
				printf("problem at write");
		}
		nbytes = write(client_endpoint,"\n",sizeof(char));
		while (1) {
			if (SIGNAL==1) {
				printf("---> Answer: %s\n",ans);
				SIGNAL=0;
				break;
			}
			else {
				sched_yield();
			}
		}
	}
	//code 2
	// char packet[200];
	// sprintf(packet, "echo \"%s\" >  %s",cmd, argv[1]);
	// printf("executing: %s\n", packet);		
	// system(packet);//sendRequest(cmd);


	// 		//sched_yield();
    




		
		//getReply(reqBuffer, &req);
		


	
	return 0;
}