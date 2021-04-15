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
	/*
	MUTEX:
		for each reqbuff:
			if reqbuff is WAIT:
				return [reqid,data]

	*/	

}

void *makeCalc(void) {
	/*
	- do the thing...
	- ret 
	*/

}

void *sendReply(void) {
	/*
	MUTEX:
		save answer at reqbuff and mark as FREE
	*/
	
}

int main(int argc, char *argv[]) {


	/*
	- CREATE STATUSES: [FREE, WAIT].
	- CREATE REQBUFF 20 eggrafwn. struct:{int reqid,int status,char *data} 

	- Create Threads "Sender" and "Receiver"

	- Create /dev/ttyS0 file, give permissions
	- have in mind to put mutexes while accessing request file
	
	while True:
		[id,data] = getRequest()
		ans=makeCalc(data)
		sendReply([id,ans])

	*/






	return 0;
}