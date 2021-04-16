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

#define TRUE 1
#define FALSE 0

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
volatile int SIGNAL_READ_FROM_ENDPOINT=TRUE; 
volatile char buff[100]="\0";


void getRequest(buffT *reqBuffer, buffT *req) {
	return;
}


void sendReply(buffT *reqBuffer, buffT *req) {
	
	return;
}


void *Sender() {

	return NULL; 
}





void *Receiver(){	
	int i,nbytes;
	while (1) {
		i = 0;
		while (SIGNAL_READ_FROM_ENDPOINT==TRUE) {
			nbytes = read(server_endpoint, buff+i, sizeof(char) );
			if (nbytes == -1)  {
				printf("Read error at writer() function.Exiting\n");
				return NULL;
    		}
    		if (buff[i]=='\n') {
    			buff[i]='\0';
    			printf("MESSAGE RECEIVED: %s\n",buff);
    			SIGNAL_READ_FROM_ENDPOINT=FALSE;
    			break;
    		}
			i++; 
    	}
    	sched_yield();
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
		tmp[0]='\0';
		while (isdigit(buff[i])) { //find multiple digits
				tmp[i-6] = buff[i];
				i++;
		}
		if (i>6) {
			num = atoi( tmp );
			if (buff[i]=='=') {
				if (buff[i+1]=='?') {//2nd command
					sprintf(ans,"0-16535");
				}
				else {
					i++;
					while (isdigit(buff[i])) { //find multiple digits
						tmp[i-7] = buff[i];
						i++;
					}
					if (i>7) {
						regs[num]=atoi(tmp);
						sprintf(ans,"OK");
					}
					else {
						sprintf(ans,"InvalidInput");
						return ans;
					}
				}
			}
			else {
				sprintf(ans,"%d",regs[num]);
			}
		}
		else {//no digits found
			sprintf(ans,"InvalidInput");			
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
	for (i=1; i < argc; i++) {
		if (argv[i][0] == '-') {
			if (strcmp(argv[i],"-nregs") == 0) {
				i++;
				parser->nregs=atoi(argv[i]);
			}
			else if (strcmp(argv[i],"-endpoint") == 0) {
				i++;
				parser->endpoint=argv[i];
			}
		}
		else {
			printf("Error: Wrong argument or argument missing or should start with \"-\" character.Type help to see the supported commands. \n");
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
	server_endpoint = open( parser.endpoint, O_RDWR | O_CREAT, S_IRWXU );
	if (server_endpoint == -1 )  {
		printf("Error opening output file. Exiting\n");
		return -1;
	}
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
		regs[i]= rand()%10;
	}

	/*
	 * MAIN FUNCTION
	 */
	int nbytes;
	while (1) {
		if (SIGNAL_READ_FROM_ENDPOINT==FALSE) {
			parseCommand(parser.nregs, regs, ans);
			// char packet[50];
			// sprintf(packet, "echo \"%s\" >  %s",ans, parser.endpoint);
			// system(packet);//sendRequest(cmd);
			for (i = 0; i < strlen(ans); i++) {
				nbytes = write(server_endpoint,ans+i,sizeof(char));
				if (nbytes == -1)
					printf("problem at write\n");
			}
			nbytes = write(server_endpoint,"\n",sizeof(char));
			SIGNAL_READ_FROM_ENDPOINT=TRUE;
			
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