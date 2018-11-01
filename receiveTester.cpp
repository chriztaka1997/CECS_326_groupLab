/*

This is a simple illustration of the use of:
	ftok, msgget, msgsnd, msgrcv

Program B creates a message queue to be shared with Program A.
Then, they will pass messages back and forth.

Program A sends the first message and reads the reply. Program A
also sends a "fake" message to the msgQ that will never be read
by Program B.

Both child processes use message type mtype = 113 and 114.

*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
using namespace std;

int main() {

	// create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);

	// declare my message buffer
	struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
		int senderID;
		int event;
		bool terminated = false;
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long);

	bool received = true;

	while(received){

	msgrcv(qid, (struct msgbuf *)&msg, size, 0, 0); // read mesg
	if(msg.senderID > 0){
						// don't read "fake" mesg
	cout << getpid() << ": gets message" << endl;
	cout << "message: " << msg.greeting << endl;
	cout << "event: " << msg.event << endl;
	strcpy(msg.greeting, " receiver ack");
	cout << getpid() << ": sends reply" << endl;
	msg.mtype = 997; // only reading mesg with type mtype = 314
	msg.senderID = 2;
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	cout << getpid() << ": now exits" << endl;
	received = false;
	}
	else{
		cout << "Waiting for message" << endl;
	}

}//end of while loop
	// now safe to delete message queue
	// msgctl (qid, IPC_RMID, NULL);

	exit(0);
}
