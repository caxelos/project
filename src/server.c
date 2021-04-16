#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sched.h>
#include <ctype.h>
#include <string.h>

#define WAIT 1
#define FREE 0
#define MAX_SIZE 20
#define _UNIX03_SOURCE

#define DISCRETE 0
#define RANGE 1

struct reqbuff {
	int id;
	int status;
	char *data;
};
typedef struct reqbuff buffT;

struct parser {
	int nregs;//number of registers
	char *endpoint;

};
typedef struct parser parserT;



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


char buff[100]="\0";
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
			//if (nbytes==0) {
			//	continue;
			//}
			if (nbytes == -1)  {
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



// int check_validity(char *cmd) {


// 	return 0;
// }


char *parseCommand(int nregs, int *regs, char *ans) {
	int i, num;
	char tmp[2]="\0";



	//do some basic validity check
	if (buff[0] != 'A' || buff[1] != 'T' || buff[2] != '+') {
		strcpy(ans,"InvalidInput");
		return ans;
		//printf("*** Input error: Commands must begin with keywork \"AT+\"\n");
	}
	


	if (buff[3] != 'R' || buff[4] != 'E' || buff[5] != 'G') {
		strcpy(ans,"InvalidInput");
		return ans;
		//printf("*** Input error: \"AT+\" must be followed by a register (eg. REG1)\n");
	}
    else {
		i=6;
		while (isdigit(buff[i])) { //find multiple digits
				tmp[i-6] = buff[i];
				i++;
		}
		if (i>6) {
			num = atoi( tmp );
			sprintf(ans,"%d",regs[num]);

		}
		else {//no digits found
			printf("READ COMMAND\n");
			
		}
		

		
	

	}


	return ans;
}

int parseArgs( parserT *parser, int argc, char *argv[]) {
	int i;

	//initialize
	parser->nregs=5;
	parser->endpoint=NULL;

	//overwrite if needed
	for (i=0; i < argc; i++) {
		if (argv[i][0] == '-') {
			if (strcmp(argv[i],"nregs") == 0) {
				i++;
				parser->nregs=atoi(argv[i]);
			}
			else if (strcmp(argv[i],"endpoint") == 0) {
				i++;
				parser->endpoint=argv[i];
			
			}
		}
		else {
			printf("Error: Arguments missing or should start with \"-\" character.Type help to see the supported commands. \n");
			return -1;
		}
	}

	if (strcmp(parser->endpoint,"\0")==0) {
		printf("Error. \"-endpoint\" argument missing (e.g. -endpoint /dev/pts1).\n");
		return -1;
	}


	return 0;
}




int main(int argc, char *argv[]) {

	//buffT req, reqBuffer[MAX_SIZE];//	buf 20 eggrafwn

	 
	pthread_t S_tid, R_tid;
	int  i;
	char ans[50];
	parserT parser;

	if (parseArgs(&parser, argc, argv) == -1)
		return 0;

	/* TODO
	- have in mind to put mutexes while accessing request file
	- Have a counter of how many requests are currently(in order to avoid concurrent bidirectional communication)
	*/



	/*
	 * INITIALIZATIONS
	 */
	//Create /dev/ttyS0 file, give permissions
	//argv[1] is the virtual port
	server_endpoint = open( parser.endpoint, O_RDWR | O_CREAT, S_IRWXU );
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
		return -1;
	}
	//Create Receiver's thread
	if (pthread_create( &R_tid,NULL, (void *)Receiver, NULL) != 0) {
		printf("Error creating thread. Exiting\n");
		return -1;
	}




	int *regs = (int *)malloc( parser.nregs*sizeof(int) );
	for (i=0; i < parser.nregs; i++) {
		regs[i]=0;
	}

	/*
	 * MAIN FUNCTION
	 */
	printf("ola oook\n");
	while (1) {
		if (SIGNAL==1) {
			SIGNAL=0;
			//getRequest(reqBuffer, &req);
			//makeCalc(&req);
			parseCommand(parser.nregs, regs, ans);

			char packet[50];
			sprintf(packet, "echo \"%s\" >  %s",ans, parser.endpoint);
			system(packet);//sendRequest(cmd);
			
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