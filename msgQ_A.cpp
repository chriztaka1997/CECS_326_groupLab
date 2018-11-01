/*

This is a simple illustration of the use of:
	ftok, msgget, msgsnd, msgrcv

Program A will use a message queue created by Program B.
Then, they will pass messages back and forth.

Program A sends the first message and reads the reply. Program A
also sends a "fake" message to the msgQ that will never be read
by anyone.

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

	// pause Program A
			// do NOT use

	int qid = msgget(ftok(".",'z'), 0);

	// declare my message buffer
	struct buf {
		long mtype; // required
		char greeting[50]; //mesg content
	};
	buf msg;
	int size = sizeof(msg)-sizeof(long);

	// sending garbage
	msg.mtype = 117;
	strcpy(msg.greeting, "Fake message");
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	//msgsnd(qid, (struct msgbuf *)&msg, size, 0);

	// strcpy(msg.greeting, "Another fake");
	// msg.mtype = -112;
	// msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	//
	// // prepare my message to send
	// strcpy(msg.greeting, "Hello there");
	// cout << getpid() << ": sends greeting" << endl;
	// msg.mtype = 118; 	// set message type mtype = 117
	// msgsnd(qid, (struct msgbuf *)&msg, size, 0); // sending
	//
	// msgrcv(qid, (struct msgbuf *)&msg, size, 314, 0); // reading
	// cout << getpid() << ": gets reply" << endl;
	// cout << "reply: " << msg.greeting << endl;
	// cout << getpid() << ": now exits" << endl;
	//
	// msg.mtype = 118;
	// msgsnd (qid, (struct msgbuf *)&msg, size, 0);

	exit(0);
}
