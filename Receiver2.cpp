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
	int qid = msgget(ftok(".",'g'), 0);

	// declare my message buffer
	struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
		int senderID; // Used to idenify a sender
		bool terminated;
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long);
	/*Each time Receiver 2 gets a message it will increment by one until it reaches 5,
	then it will send a termination message
	*/
	int counter = 0;
	cout << "Start here:" << endl;

	while(counter < 5000)
	{
		/* This receive statement will look for messages from 997 and 257.
		Both 997 and 257 will contain mtype 100
		Send-ID will contain a unique number represents a single sender.
		*/
		msgrcv(qid, (struct msgbuf *)&msg, size, 100,0);
		cout << getpid() << ": gets message" << endl;
		cout << "message: " << msg.greeting << endl;
		counter += 1;
		cout << "current counter: " << counter << endl;

		/*
		  After a receive statement is complete, the counter is incremented
		  If the senderId is 997 then the receiver will send an acknowledgement
		  If the senderId is 257 then the receiver will just increment the counter
		*/
		int id = msg.senderID;
		cout << "current Sender ID: " << id << endl;
		if(id == 997)
		{
			strcat(msg.greeting, " and Adios.");
			msg.mtype = 997;
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);

		}
		cout<<endl;

	}
	cout << "END IT" << endl;
	//Send Termination message
	strcat(msg.greeting, "DONE");
	msg.mtype = 99;
	msg.terminated = true;
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

	exit(0);
}
