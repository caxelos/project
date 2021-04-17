#include <serverMW.h>



/*
 * - deviceFd: file descriptor of a /dev/pts device on server side.
 *
 * - SIGNAL_READ_FROM_DEVICE: a signal to allow read or not from serial port
 *
 * - SIGNAL_WRITE_TO_DEVICE: a signal to allow read or not from serial port
 *
 * - cmd: command received from client throught serial virtual port
 *
 * - ans: answer sent from server to client through serial virtual port
 *
 * - regfile: stores dynamically the registers' data
 */
volatile int deviceFd;
volatile int SIGNAL_READ_FROM_DEVICE=TRUE;
volatile int SIGNAL_WRITE_TO_DEVICE=FALSE; 
volatile char cmd[MAX_SIZE]="\0";
volatile char ans[MAX_SIZE]="\0";
volatile unsigned short *regfile;




/*
 * Thread Sender()
 *
 * - Sends data through device serial port to client
 *
 * - Communicates with main thread through SIGNAL_WRITE_TO_DEVICE
 *
 * - Yields the processor if there aren't any data to write to serial port
 */
void *Sender() {
	int i, nbytes;

	while (1) {
		while (SIGNAL_WRITE_TO_DEVICE==TRUE) {
			for (i = 0; i < strlen(ans); i++) {
				nbytes = write(deviceFd,ans+i,sizeof(char));
				if (nbytes == -1) {
					printf("problem at write. Exiting.\n");
					exit(-1);
				}
			}
			nbytes = write(deviceFd,"\n",sizeof(char));
			if (nbytes == -1) {
				printf("problem at write. Exiting.\n");
				exit(-1);
			}

			SIGNAL_WRITE_TO_DEVICE=FALSE;
		}
		sched_yield();	
	}

	return NULL; 
}


/*
 * Thread Receiver()
 *
 * - Receives data from client through device serial port
 *
 * - Communicates with main thread through SIGNAL_READ_FROM_DEVICE
 *
 * - Yields the processor if there aren't any data to write to serial port
 */
void *Receiver(){	
	int i,nbytes;
	while (1) {
		i = 0;
		while (SIGNAL_READ_FROM_DEVICE==TRUE) {
			nbytes = read(deviceFd, cmd+i, sizeof(char) );
			if (nbytes == -1)  {
				printf("Read error. Exiting\n");
				exit(-1);
    		}
    		if (cmd[i]=='\n') {
    			cmd[i]='\0';
    			printf("Command \"%s\" received.\n",cmd);
    			SIGNAL_READ_FROM_DEVICE=FALSE;
    			break;
    		}
			i++; 
    	}
    	sched_yield();
	}

	return NULL;
};



/*
 * Function parseCommand()
 *
 * - Checks validity, interprets commands and creates answers 
 *
 * - Reads/Write from/to registers
 *
 */
char *parseCommand(int *nregs, char *ans) {
	int i,j, num,num_tmp;
	char tmp2[10];


	/*
	 * "add-register" command
	 */
	if (strcmp(cmd,"add-register")==0) {
		*nregs = *nregs + 1;
		regfile = (unsigned short*)realloc(regfile, (*nregs)  *sizeof(unsigned short));
		if (regfile == NULL) {
			printf("Error allocating memory. Exiting\n");
			exit(-1);
		}
		regfile[*nregs-1] = rand()%10;
		sprintf(ans,"\"REG%d\" added in registers. Total registers:%d", *nregs, *nregs);
		return ans;
	}

	/*
	 * do some basic validity check
	 */
	if (cmd[0] != 'A' || cmd[1] != 'T' || cmd[2] != '+' || cmd[3] != 'R' || cmd[4] != 'E' || cmd[5] != 'G' || strlen(cmd)<7 ) {
		sprintf(ans,"InvalidInput");
		return ans;
	}

	/*
	 * check if digit follows
	 */
	num=atoi(cmd+6);
	if (num == 0) {
		sprintf(ans,"InvalidInput");
	}
    else {
    	
    	i=6;

    	/*
    	 * find num of digits
    	 */
    	num_tmp=num;
    	while (num_tmp != 0) {
    		num_tmp /= 10;     
    		i++;	
    	}
    	

    	/*
    	 * check for wrong typed "=?" command
    	 */
       	if (strchr(cmd,'?') != NULL && *(strchr(cmd,'?')-1) != '=' ) {
       		sprintf(ans,"InvalidInput");
    	}
    	else {

    		/*
    		 * remove all spaces
    		 */
	    	j=i; 
	    	num_tmp=i;
	    	for (; i<strlen(cmd);i++) {
	    		if (cmd[i] != ' ')
	    			cmd[j++] = cmd[i];
	    	}
	    	cmd[j]='\0';
	    	i=num_tmp;
	    	

			if (cmd[i]=='=') {			
				if (cmd[i+1]=='?') { 

					/*
					 * "=?" command
					 */ 
					sprintf(ans,"0-%hu",(unsigned short)-1);
				}
				else {
					i++;
					num_tmp=atoi(cmd+i);
					sprintf(tmp2,"%d",num_tmp);	
					if ( strcmp(cmd+i,tmp2)==0 ) {
						if (num > *nregs) {

							/*
							 * wrong Register number
							 */
							sprintf(ans,"Error: Register numbers must be from \"1\" to \"%d\"", *nregs);
						}
						else {

							/*
							 * AT+REG<num>=<num> command
							 */
							regfile[num-1]=num_tmp;
							sprintf(ans,"OK");
						}
					}
					else {
						sprintf(ans,"InvalidInput");
					}
				}
			}
			else if (cmd[i]=='\0' &&  (num <= *nregs) && (num > 0) ) {

				/*
				 * AT+REG<num> command
				 */
				sprintf(ans,"%d", regfile[num-1]);
			}
			else {

				/*
				 * no digits found
				 */
				sprintf(ans,"InvalidInput");
			}
		}
	}


	return ans;
}


/* function initialize_server()
 *
 * - Creates file descriptors for virtual serial device, eg. /dev/pts/0
 *
 * - Creates threads Sender and Receiver
 *
 * - Allocates dynamically the registers
 *
 */
int initialize_server(parserT *parser) {
	pthread_t S_tid, R_tid;
	int i;
	

	/*
	 * Open file descriptor for serial device 
	 */
	deviceFd = open( parser->endpoint, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU );
	if (deviceFd == -1 )  {
		printf("Error opening output file. Exiting\n");
		exit(-1);
	}


	/*
	 * Create Senders's thread
	 */
	if (pthread_create( &S_tid,NULL, (void *)Sender, NULL) != 0) {
		printf("Error creating thread. Exiting\n");
		exit(-1);
	}

	/*
	 * Create Receiver's thread
	 */
	if (pthread_create( &R_tid,NULL, (void *)Receiver, NULL) != 0) {
		printf("Error creating thread. Exiting\n");
		exit(-1);
	}

	
	/*
	 * Allocate registers dynamically and initialize them randomly
	 */
	regfile = (unsigned short *)malloc( parser->nregs*sizeof(unsigned short) );
	if (regfile==NULL) {
		printf("Error allocating memory. Exiting\n");
		exit(-1)
	}
	for (i=0; i < parser->nregs; i++) {
		regfile[i]= rand()%10;
	}

	return 0;

}



/* function run_server()
 *
 * - gives signals to threads Sender,Receiver when to read/write from serial port
 *
 * - parses the command received from client
 *
 * - yields the processor when no need
 *
 */
int run_server(parserT *parser)  {
	
	while (1) {
		if (SIGNAL_READ_FROM_DEVICE==FALSE) {
			parseCommand( &(parser->nregs), ans);
			SIGNAL_WRITE_TO_DEVICE=TRUE;

			while (SIGNAL_WRITE_TO_DEVICE==TRUE) {
				sched_yield();
			}
			SIGNAL_READ_FROM_DEVICE=TRUE;
		}
		else {
			sched_yield();
		}
	}
	
	

	return 0;

}


/* function close_server()
 * 
 * - closes serial's port file descriptor
 *
 * - frees dynamically allocated memory for registers
 */
int close_server() {
	if (close(deviceFd) < 0) {
		printf("error closing server endpoint. Exiting\n");
		return -1;
	}
	free( regfile );

	return 0;
}
