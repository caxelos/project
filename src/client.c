#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define MSG_SIZE 100

//volatile char ANS_NONCACHED[MSG_SIZE]="\0";
volatile char ans[MSG_SIZE]="\0";
volatile int SIGNAL_READ_FROM_ENDPOINT=FALSE;

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
	//char ans[MSG_SIZE];
	i=0;
	while (1) {
		i = 0;
		while (SIGNAL_READ_FROM_ENDPOINT==TRUE) {
			nbytes = read(client_endpoint, ans+i, sizeof(char) );
			if (nbytes == -1)  {
				printf("Read error at writer() function.Exiting\n");
				return NULL;
    		}
    		if (ans[i]=='\n') {
    			ans[i]='\0';
    			i=0;
    			//strcpy(ANS_NONCACHED,(volatile char *)ans);
    			SIGNAL_READ_FROM_ENDPOINT=FALSE;
    		}
    		else {
    			i++;
    		}
    	}	
    	sched_yield();
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
    //char ans[100]="\0";
	printf("Welcome to the app.\n");
	while (1) {
		printf("\n<--- Please enter command: ");
		scanf("%s", cmd);
		for (i = 0; i < strlen(cmd); i++) {
			nbytes = write(client_endpoint,cmd+i,sizeof(char));
				if (nbytes == -1)
					printf("problem at write");
		}
		nbytes = write(client_endpoint,"\n",sizeof(char));
		SIGNAL_READ_FROM_ENDPOINT=TRUE;

		while (1) {
			if (SIGNAL_READ_FROM_ENDPOINT==FALSE) {
				//printf("copying \"%s\" to \"%s\"\n",ANS_NONCACHED,ans);
				//strcpy(ans,(char *)ANS_NONCACHED);

				printf("---> Answer: %s\n",ans);
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