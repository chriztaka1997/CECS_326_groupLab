#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <thread>
#include <chrono>

using namespace std;

//This will be reandom generator that will be assign to msg.event
int randomGenerator(){
	int randomNumber = 0;
	randomNumber = rand()%4294967296 + 1; //random number from 1 to 2^32
	return randomNumber;
}

//This the method that will be use when message interrupt
void times_up(int s){
	cout<<"Termination message not found"<<endl;
	cout<<endl;
	return;
}

//Main function
int main() {

  int qid = msgget(ftok(".",'c'), 0);

  struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
		int senderID;
		int event;
		bool terminated;
  };

  buf msg;
  int size = sizeof(msg)-sizeof(long);

  msg.mtype = 100;				//assigning mtype to 100
	msg.senderID = 257;
	msg.terminated = false;	//Assign to folse because it is not going to terminate
  strcpy(msg.greeting,"This is 257");

  bool send = true;


  int count = 0;				//This will be the counter to check terminationof receiver 2
  int end_count = 5000; //Check termination of receiver 2 after 5000 message send

	int timer = 0;
	signal(SIGALRM,times_up); //This is for handling the interrupt signal from the alarm

  while (send){

		cout<<"Send message to receiver 2"<<endl; //debug

		msg.event = randomGenerator();
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);


		alarm(1); //Interrupt the message receive after 1 second
		msgrcv(qid,(struct msgbuf *)&msg,size,99,0); //if receiver 2 terminated


		if(msg.mtype == 99){
			cout<<"Termination message received"<<endl;
			send = false;	//Get out of the loop
		}
	}

	//Send termination message to the message queue
	strcpy(msg.greeting,"257 terminated");
	msg.terminated = true;
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

	cout<<"Terminating";

}
