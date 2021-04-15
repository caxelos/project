/* Sender thread

	while True:
		MUTEX:
			for each status:
				if status==DONE:
					- Pack and Send [reqid,Result] (write to /dev/ttyS0)
					- Make status FREE 

*/



/* Receiver thread
	
	while True:
	  - wait until /dev/ttyS0 has data
	  - unpack data:[reqid,data]

	  MUTEX: 
	  	- save them: [reqid, data, status=WAIT]

*/


void *getRequest(void) {

}

void *makeCalc(void) {

}

void *sendReply(void) {
	
}

int main(int argc, char *argv[]) {



	/*
	- CREATE STATUSES: [FREE, WAIT, DONE].

	- Create Threads "Sender" and "Receiver"

	- Create /dev/ttyS0 file, give permissions
	- have in mind to put mutexes while accessing request file
	
	- callfunctions getRequest(), makeCalc() and sendReply()

	*/






	return 0;
}