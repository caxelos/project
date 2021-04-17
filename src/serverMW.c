#include <serverMW.h>



/*
 * deviceFd: file descriptor of a /dev/pts device on server side.
 * 
 */
volatile int deviceFd;
volatile int SIGNAL_READ_FROM_DEVICE=TRUE;
volatile int SIGNAL_WRITE_TO_DEVICE=FALSE; 
volatile char buff[MAX_SIZE]="\0";
volatile char ans[MAX_SIZE]="\0";
volatile unsigned short *regfile;




void *Sender() {
	int i, nbytes;

	while (1) {
		while (SIGNAL_WRITE_TO_DEVICE==TRUE) {
			for (i = 0; i < strlen(ans); i++) {
				nbytes = write(deviceFd,ans+i,sizeof(char));
				if (nbytes == -1)
					printf("problem at write\n");
			}
			nbytes = write(deviceFd,"\n",sizeof(char));
			SIGNAL_WRITE_TO_DEVICE=FALSE;
		}
		sched_yield();	
	}

	return NULL; 
}


void *Receiver(){	
	int i,nbytes;
	while (1) {
		i = 0;
		while (SIGNAL_READ_FROM_DEVICE==TRUE) {
			nbytes = read(deviceFd, buff+i, sizeof(char) );
			if (nbytes == -1)  {
				printf("Read error at writer() function.Exiting\n");
				return NULL;
    		}
    		if (buff[i]=='\n') {
    			buff[i]='\0';
    			printf("command \"%s\" received.\n",buff);
    			SIGNAL_READ_FROM_DEVICE=FALSE;
    			break;
    		}
			i++; 
    	}
    	sched_yield();
	}

	return NULL;
};




char *parseCommand(int *nregs, char *ans) {
	int i,j, num,num_tmp;
	char tmp2[10];



	if (strcmp(buff,"add-register")==0) {
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
	 *do some basic validity check
	 */
	if (buff[0] != 'A' || buff[1] != 'T' || buff[2] != '+' || buff[3] != 'R' || buff[4] != 'E' || buff[5] != 'G' || strlen(buff)<7 ) {
		sprintf(ans,"InvalidInput");
		return ans;
	}


	//all commands here start with "AT+REG"
	//AT+REG 1 - INVALID
	//AT+REG1 3 - INVALID 	


	num=atoi(buff+6);
	if (num == 0) {
		sprintf(ans,"InvalidInput");
	}
    else {
    	
    	i=6;
    	//find num of digits
    	num_tmp=num;
    	while (num_tmp != 0) {
    		num_tmp /= 10;     
    		i++;	
    	}
    	

    	//check "=?" command
       	if (strchr(buff,'?') != NULL && *(strchr(buff,'?')-1) != '=' ) {
       		printf("popa\n");
       		sprintf(ans,"InvalidInput");
    	}
    	else {
    		// remove all spaces
	    	j=i; 
	    	num_tmp=i;
	    	for (; i<strlen(buff);i++) {
	    		if (buff[i] != ' ')
	    			buff[j++] = buff[i];
	    	}
	    	buff[j]='\0';
	    	i=num_tmp;
	    	

			if (buff[i]=='=') {			
				if (buff[i+1]=='?') {//2nd command
					sprintf(ans,"0-%hu",(unsigned short)-1);
				}
				else {
					i++;
					num_tmp=atoi(buff+i);
					sprintf(tmp2,"%d",num_tmp);	
					if ( strcmp(buff+i,tmp2)==0 ) {
						if (num > *nregs) {
							sprintf(ans,"Error: Register numbers must be from \"1\" to \"%d\"", *nregs);
						}
						else {
							regfile[num-1]=num_tmp;
							sprintf(ans,"OK");
						}
					}
					else {
						sprintf(ans,"InvalidInput");
					}
				}
			}
			else if (buff[i]=='\0' &&  (num <= *nregs) && (num > 0) ) {
				sprintf(ans,"%d", regfile[num-1]);
			}
			else {//no digits found
				sprintf(ans,"InvalidInput");
			}
		}
	}


	return ans;
}


int initialize_server(parserT *parser) {
	pthread_t S_tid, R_tid;
	int i;
	

	//Open file descriptor for serial device 
	deviceFd = open( parser->endpoint, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU );
	if (deviceFd == -1 )  {
		printf("Error opening output file. Exiting\n");
		return -1;
	}


	//Create Senders's thread
	if (pthread_create( &S_tid,NULL, (void *)Sender, NULL) != 0) {
		printf("Error creating thread. Exiting\n");
		return -1;
	}

	//Create Receiver's thread
	if (pthread_create( &R_tid,NULL, (void *)Receiver, NULL) != 0) {
		printf("Error creating thread. Exiting\n");
		return -1;
	}

	regfile = (unsigned short *)malloc( parser->nregs*sizeof(unsigned short) );
	for (i=0; i < parser->nregs; i++) {
		regfile[i]= rand()%10;
	}

	return 0;

}





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

int close_server() {
	if (close(deviceFd) < 0) {
		printf("error closing server endpoint. Exiting\n");
		return -1;
	}
	free( regfile );

	return 0;
}
