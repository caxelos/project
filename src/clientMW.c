
#include <clientMW.h>

volatile int SIGNAL_READ_FROM_ENDPOINT=FALSE;
volatile int client_endpoint;
volatile char ans[MSG_SIZE]="\0";

void *Sender() {
	return NULL;

}


void *Receiver(){
	int i,nbytes=0;	
	i=0;


	if (read(client_endpoint, ans+i, sizeof(char)==0))
		printf("cleared ok!\n");


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
    			printf("received:%s\n",ans);
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

void printATcommands(void) {
	
	printf("Available AT Commands:\n");
	printf("1) REG1: Read the register-1 value -> Response: <int>.\n");
	printf("2) REG1=?: Range of discrete values for register-1 (eg.0-%hu).\n",(unsigned short)-1);
	printf("3) REG1=4: Store value \"4\" in register-1.\n");
	printf("4) add-register: Add one more register in register's list.\n");

}


int run_client() {
	char cmd[100];
	int i,nbytes;


	printf("Welcome to the app.\n");
	while (1) {
		printf("~  ");//printf("\n<--- Please enter command: ");
		scanf("%s", cmd);
		if (strcmp(cmd,"help") == 0) {
			printATcommands();
			continue;
		}

		for (i = 0; i < strlen(cmd); i++) {
			nbytes = write(client_endpoint,cmd+i,sizeof(char));
			if (nbytes == -1) {
				printf("problem at write");
			}
		}
		nbytes = write(client_endpoint,"\n",sizeof(char));
		SIGNAL_READ_FROM_ENDPOINT=TRUE;

		while (1) {
			if (SIGNAL_READ_FROM_ENDPOINT==FALSE) {
				printf("%s\n",ans);
				break;
			}
			else {
				sched_yield();		
			}
		}
	}

	return 0;
}


int initialize_client(char *argv[]) {
	pthread_t R_tid;


	client_endpoint = open( argv[1], O_RDWR | O_CREAT, S_IRWXU );
	if (client_endpoint == -1 )  {
		printf("Error opening output file. Exiting\n");
		exit(-1);
	}

	if (access(argv[1], X_OK)==0) {
		if (truncate(client_endpoint,0)==-1) {
			printf("error clearing %s device. Exiting\n", argv[1] );
			exit(-1);
		}
	}



	//Create sender's thread
	// if (pthread_create( &S_tid,NULL, (void *)Sender, NULL) != 0) {
	// 	printf("Error creating thread. Exiting\n");
	// 	return -1;
	// }

	//Create Receiver's thread
	if (pthread_create( &R_tid,NULL, (void *)Receiver, NULL) != 0) {
		printf("Error creating thread. Exiting\n");
		return -1;
	}


}


int close_client() {
	if (close(client_endpoint) < 0) {
		printf("error closing server endpoint. Exiting\n");
		return -1;
	}
	return 0;

}