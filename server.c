#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sched.h>

#define WAIT 1
#define FREE 0
#define MAX_SIZE 20
#define _UNIX03_SOURCE


struct reqbuff {
	int id;
	int status;
	char *data;
};
typedef struct reqbuff buffT;


volatile int server_endpoint;

/* Sender thread

	while True:
		MUTEX:
			for each status:
				if status==DONE:
					- Pack and Send [reqid,Result] (write to /dev/ttyS0)
					- Make status FREE 

*/






void getRequest(buffT *reqBuffer, buffT *req) {
	/*
	MUTEX:
		for each reqbuff:
			if reqbuff is WAIT:
				return [reqid,data]

	*/	
	static int i=0;
	while (1) {
		if (reqBuffer[i].status==WAIT) {
			req=reqBuffer;
			break;
		}

		i = (i+1)%MAX_SIZE;
	}

	return;
}

void makeCalc(buffT *req) {
	//req.data contains the command
	printf("executed command is:%s\n", req->data);



	/*
	- do the thing...
	- ret 
	*/

	return;
}

void sendReply(buffT *reqBuffer, buffT *req) {
	/*
	MUTEX:
		save answer at reqbuff and mark as FREE
	*/


	return;
}


void *Sender() {

	return NULL; 
}


char buff[100];
int SIGNAL=0; 

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
			nbytes = read(server_endpoint, buff+i, sizeof(char) );
			if (nbytes==0) {
				SIGNAL=0;
				sched_yield();
				printf("yield processor...\n");
				continue;
			}
			else if (nbytes == -1)  {
				printf("Read error at writer() function.Exiting\n");
				return NULL;
    		}

    		if (buff[i]=='\n') {
    			buff[i]='\0';
    			printf("MESSAGE RECEIVED: %s\n",buff);
    			SIGNAL=1;
    			break;
    		}
    	


			i++; 
			//sched_yield();
    	}

	}


	return NULL;
}

int main(int argc, char *argv[]) {

	//buffT req, reqBuffer[MAX_SIZE];//	buf 20 eggrafwn
	 
	pthread_t S_tid, R_tid;
	//int  i;

	/* TODO
	- have in mind to put mutexes while accessing request file
	- Have a counter of how many requests are currently(in order to avoid concurrent bidirectional communication)
	*/



	/*
	 * INITIALIZATIONS
	 */
	//Create /dev/ttyS0 file, give permissions
	//argv[1] is the virtual port
	server_endpoint = open( argv[1], O_RDWR | O_CREAT, S_IRWXU );
	if (server_endpoint == -1 )  {
		printf("Error opening output file. Exiting\n");
		return -1;
   }


   //  //init reqBuffer
   // for (i=0; i < MAX_SIZE; i++) {
   // 	reqBuffer[i].status=FREE;
   // 	reqBuffer[i].data=NULL;
   // }



   	/*
   	 * THREADS
   	 */

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
	while (1) {
		if (SIGNAL==1) {
			SIGNAL=0;
			//getRequest(reqBuffer, &req);
			//makeCalc(&req);
			printf("Got command: %s\n", buff);
			//system("echo \"it's alright\" > %s", argv[1]);//sendReply(argv[1]);
		}
		else {
			sched_yield();
		}
	}





	if (close(server_endpoint) < 0) {
		printf("error closing server endpoint. Exiting\n");
		return -1;
	}
	return 0;
}