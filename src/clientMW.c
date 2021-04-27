#include <clientMW.h>


/*
 * - deviceFd: file descriptor of a /dev/pts device on client side.
 *
 * - SIGNAL_READ_FROM_DEVICE: a signal to allow read or not from serial port
 *
 * - SIGNAL_WRITE_TO_DEVICE: a signal to allow read or not from serial port
 *
 * - cmd: command sent to server throught serial virtual port
 *
 * - ans: answer received from server through serial virtual port
 *
 * - regfile: stores dynamically the registers' data
 */

volatile int SIGNAL_READ_FROM_DEVICE=FALSE;
volatile int SIGNAL_WRITE_TO_DEVICE=FALSE;
volatile int deviceFd;
volatile char ans[MSG_SIZE]="\0";
volatile char cmd[MSG_SIZE]="\0";



/*
 * Thread Sender()
 *
 * - Sends data through device serial port to server
 *
 * - Communicates with main thread through SIGNAL_WRITE_TO_DEVICE
 *
 * - Yields the processor if there aren't any data to write to serial port
 */
void *Sender() {
	int i,nbytes;

	while (1) {
		while (SIGNAL_WRITE_TO_DEVICE==TRUE) {
			for (i = 0; i < strlen(cmd); i++) {
				nbytes = write(deviceFd,cmd+i,sizeof(char));
				if (nbytes == -1) {
					printf("problem at write. Exiting\n");
					exit(-1);
				}
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
 * - Receives data from server through device serial port
 *
 * - Communicates with main thread through SIGNAL_READ_FROM_DEVICE
 *
 * - Yields the processor if there aren't any data to write to serial port
 */
void *Receiver(){
	int i,nbytes=0;	
	i=0;

	while (1) {
		i = 0;
		while (SIGNAL_READ_FROM_DEVICE==TRUE) {
			nbytes = read(deviceFd, ans+i, sizeof(char) );
			if (nbytes == -1)  {
				printf("Read error at writer() function.Exiting\n");
				return NULL;
    		}
    		if (ans[i]=='\n') {
    			ans[i]='\0';
    			i=0;
    			SIGNAL_READ_FROM_DEVICE=FALSE;
    		}
    		else {
    			i++;
    		}
    	}	
    	sched_yield();
	}


	return NULL;
}


/* function printATcommands()
 *
 * - prints the available commands 
 *
 */
void printATcommands(void) {
	
	printf("Available AT Commands:\n");
	printf("1) REG1: Read the register-1 value -> Response: <int>.\n");
	printf("2) REG1=?: Range of discrete values for register-1 (eg.0-%hu).\n",(unsigned short)-1);
	printf("3) REG1=4: Store value \"4\" in register-1.\n");
	printf("4) add-register: Add one more register in register's list.\n");

}


/* function run_client()
 *
 * - prints message to CLI to provide assistance to user
 *
 * - gives signals to threads Sender,Receiver when to read/write from serial port
 *
 * - yields the processor when no need
 *
 */
int run_client() {

	printf("Welcome to the app.\nTo terminate, press \"exit\".\n\n");
	while (1) {

		printf("~  ");
		fgets (cmd, MSG_SIZE , stdin);
		if (strncmp(cmd,"help",4) == 0) {
			printATcommands();
			continue;
		}
		else if ( strncmp(cmd,"exit",4)==0)
			break;

		SIGNAL_WRITE_TO_DEVICE=TRUE;		
		while (SIGNAL_WRITE_TO_DEVICE==TRUE) {
			sched_yield();
		}

		SIGNAL_READ_FROM_DEVICE=TRUE;
		while (SIGNAL_READ_FROM_DEVICE==TRUE) {
			sched_yield();
		}
		printf("%s\n",ans);

	
	}

	return 0;
}


/* function initialize_client()
 *
 * - Creates file descriptors for virtual serial device, eg. /dev/pts/0
 *
 * - Creates threads Sender and Receiver
 *
 */
int initialize_client(parserT *parser) {
	pthread_t S_tid,R_tid;

	deviceFd = open( parser->endpoint, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU );
	if (deviceFd == -1 )  {
		printf("Error opening output file. Exiting\n");
		exit(-1);
	}	

	if (pthread_create( &S_tid,NULL, (void *)Sender, NULL) != 0) {
		printf("Error creating thread. Exiting\n");
		return -1;
	}

	if (pthread_create( &R_tid,NULL, (void *)Receiver, NULL) != 0) {
		printf("Error creating thread. Exiting\n");
		return -1;
	}

	return 0;
}


/* function close_client()
 * 
 * - closes serial's port file descriptor
 *
 */
int close_client() {
	if (close(deviceFd) < 0) {
		printf("error closing server endpoint. Exiting\n");
		return -1;
	}
	return 0;

}